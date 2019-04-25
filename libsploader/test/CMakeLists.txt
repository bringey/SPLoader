project("test_libsploader" C)

set(TEST_LIBSPLOADER_CFLAGS -std=gnu11)
project_template()
test_template("libsploader")

# any complex tests that cannot be created by the template should go here
