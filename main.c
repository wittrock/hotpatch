#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

size_t test_sym = 42;

int
one(void)
{

	return 42;
}

int
main()
{

	for (int i = 0; i < 1; i++) {
		int ret = one();
		printf ("ret %d\n", ret);
	}

	void *handle = dlopen("test.so", RTLD_LAZY | RTLD_LOCAL | RTLD_NODELETE);
	if (handle == NULL) {
	  printf("could not open lib: %s\n", dlerror());
	  return -1;
	}

	void *own_handle = dlopen(NULL, RTLD_NOW | RTLD_GLOBAL);
	if (own_handle == NULL) {
	  printf("could not resolve own symbols: %s\n", dlerror());
	  return -1;
	}

	void *magic_sym_addr = dlsym(handle, "magic");
	assert(magic_sym_addr != NULL && "could not find magic sym.");

	void *magic_sym_len_addr = dlsym(handle, "magic_len");
	assert(magic_sym_len_addr != NULL && "could not find magic length");

	size_t magic_sym_len = *((size_t *)magic_sym_len_addr);
	char **magic_sym = (char **)magic_sym_addr;

	printf("number of magic symbols: %zu\n", magic_sym_len);

	for (int i = 0; i < magic_sym_len; i++) {
		printf("looking for symbol %s.\n", magic_sym[i]);
		/* XXX Find symbol in this executable, replace it. */
		void *own_addr = dlsym(own_handle, magic_sym[i]);
		if (own_addr == NULL) {
		  printf("Could not find symbol %s from magic in own executable: %s. Continuing to next sym.\n", magic_sym[i], dlerror());
		  continue;
		}

		void *new_addr = dlsym(handle, magic_sym[i]);
		if (new_addr == NULL) {
		  printf("Could not find symbol %s from magic in new shared lib: %s. Continuing to next sym.\n", magic_sym[i], dlerror());
		  continue;
		}

		printf("want to jump to: %p\n", new_addr);
		/* the relative address takes effect _after_ the length of the opcode and arg, which is 5 */
		uint8_t jmp = 0xE9;
		uint32_t rel_addr = (uint32_t)(((uintptr_t)new_addr) - ((uintptr_t)own_addr)) - 5;

		printf("relative jmp address: %x\n", rel_addr);
		/* XXX: assert rel_addr has no significant bits other than the lower 31 and the sign bit. */

		uint8_t new_contents[8];
		memcpy(&new_contents[0], &jmp, sizeof(uint8_t));
		memcpy(&new_contents[1], &rel_addr, sizeof(uint32_t));
		printf("new_contents without old: %p\n", *((void **)&new_contents));
		printf("relative jmp address from new_content: %x\n", *((uint32_t *)&new_contents[1]));

		/* 3 bytes we need to keep from the old address */
		memcpy(&new_contents[5], own_addr + 5, 3);
		  
		printf("new_contents with old: %p\n", *((void **)&new_contents));

		void *nearest_page_down = (void *)((uintptr_t)own_addr & ~((1 << 22) - 1));
		printf("nearest page down from own_addr %p: %p\n", own_addr, nearest_page_down);
		
		/* Really this instruction might be on a page boundary. */
		int ret = mprotect(nearest_page_down, 4096, PROT_READ|PROT_WRITE|PROT_EXEC);
		if (ret != 0) {
		  char *error = strerror(errno);
		  printf("could not mprotect the right address: %s\n", error);
		}

		/* THIS IS DANGEROUS */
		memcpy(own_addr, &new_contents, 8);
		
		/* XXX: create a function which jumps to an arbitrary global address. jump to that function. */

		/* XXX: mprotect back */
		
		printf("Resolved symbol %s\n", magic_sym[i]);
	}
	
	for (int i = 0; i < 1; i++) {
		int ret = one();
		printf ("ret %d\n", ret);
	}

	return 0;
}

