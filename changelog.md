# Changelog

## v1.1
- Added Doxyfile and doxygen-style file descriptions.
- Added function `string_copy_scoped` to copy scoped string using the default allocator.
- CHANGE DEFAULT ALLOCATOR!!!
    - its ok to change because each object will store its own allocator.

## v1.0
- First version.
- Array, Hashmap, and String types.
- Support for 32bit and 64bit architectures as well as a no standard library.