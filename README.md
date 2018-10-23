# SPLoader
Systems Programming bootLoader

SPLoader is a rewrite of the CSCI-452 Systems Programming bootloader. The
CSCI-452 bootloader was written for the i386-pc platform and is capable of
loading flat kernel/program binaries at specified memory locations.

SPLoader aims to add additional features while remaining minimal as possible.

## Getting Started

In order to build SPLoader, you will need a cross-compiler setup. The GCC
toolchain is the only one supported by this build system. The default
configuration will build for the i386-pc platform, which requires binutils and
gcc for the i686-elf target. Latest stable versions for GCC and binutils are
recommended.

Requirements:
 * GCC
 * Binutils
 * CMake
 * ninja-build or GNU Make

### Building

```sh
# for i386-pc target
cd build
cmake ../ -G Ninja  # ninja is preferred, but make can be used instead
ninja

# for other targets (none available right now)
cd build
cmake -D TARGET=<arch>-<platform> ../ -G Ninja
ninja
```

## Contributing

Read the [CONTRIBUTING.md](CONTRIBUTING.md) file for details on the build
system, coding conventions/style and how to submit pull requests.

## Versioning

This project uses Semantic Versioning v2.0.0.

Currently, the project is in development phase (major version 0). See the
[CONTRIBUTING.md](CONTRIBUTING.md) file for details on tagging new versions.

## Authors

 * Brennan Ringey - Project lead - [bringey](https://github.com/bringey)

## License

This project is licensed under the MIT License - See the [LICENSE](LICENSE) file
for more details.

## Acknowledgments

 * Thanks to Jon Coles, Warren R. Carithers, K. Reek and Garrett C. Smith for
   the original bootloader this project is based on.
