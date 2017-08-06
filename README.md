# SPLoader
Systems Programming bootLoader

SPLoader is a rewrite of the CSCI-452 Systems Programming bootloader. The
CSCI-452 bootloader was written for the i386-pc platform and is capable of
loading flat kernel/program binaries at specified memory locations.

SPLoader aims to add additional features while remaining minimal as possible.

## Getting Started

In order to build SPLoader, you will need a cross-compiler setup. The default
configuration will build for the i386-pc platform, which requires binutils and
gcc for the i686-elf target. GCC 7.1.0 and Binutils 2.29 are recommended.

### Building

Currently, the only target to build is a disk image for testing purposes
```sh
make build/usb.img
```

Which can then be run in qemu using the [qemu](qemu) script:
```sh
./qemu
```
Or by invoking qemu manually:
```sh
qemu-system-i386 -drive file=build/usb.img,format=raw
```

## Contributing

Read the [CONTRIBUTING](CONTRIBUTING) file for details on the build system,
coding conventions/style and how to submit pull requests.

## Versioning

TBD - this project is under development, this section will be updated once the
project is somewhat usable.

## Authors

 * Brennan Ringey - Project lead - [pedgeon](https://github.com/pedgeon)

## License

This project is licensed under the GNU General Public License v3.0 - See the
[LICENSE](LICENSE) file for more details

## Acknowledgments

 * Thanks to Jon Coles, Warren R. Carithers, K. Reek and Garrett C. Smith for
   the original bootloader this project is based on.
