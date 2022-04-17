# STM32 CRC32 software calculation utility

A software algorithm that computes the same CRC32 of the hardware STM32 CRC peripheral of the input binary.

Algorithm taken from the website https://www.cnblogs.com/shangdawei/archive/2013/04/28/3049789.html.

## STM32 CRC peripheral problem

The hardware STM32 CRC module works non-standard, i.e. different from the classic CRC32 calculation algorithm. The algorithm performs a bitwise permutation, both at the input and at the output, plus an inversion also occurs at the output. But this can be solved by adding a some code that the STM32CubeHAL has:

```
LL_CRC_FeedData32(CRC, __REV(Data));
Crc = __REV(LL_CRC_ReadData32(CRC)) ^ 0xFFFFFFFF;
```

Also, most common utilities for calculating CRC32 in Windows (such as Total Commander, WinRAR, WinZIP ...) and most of the available algorithms on the Internet calculates not the "classic" CRC32, but its variety: `CRC32B`.

Comparison of hash sums for ASCII input value `123456789`:

	+---------------+----------------+
	| crc32	       |     0x181989FC |
	+---------------+----------------+
	| crc32b        |     0xCBF43926 |
	+---------------+----------------+
	| crc32 stm32   |     0x61706427 |
	+---------------+----------------+

Thus, on the one hand, there is a hardware module for calculating CRC32 in stm32, but most common utilities are not suitable for working with it.

`stm32-crc32` utility is software version of the CRC calculation algorithm, which calculates CRC32 of input binary file with any length. This utility can be used to calculate CRC32 of firmware for STM32. You can use the utility source code to write your own scripts, which, for example, automatically fills the custom binary image headers with calculated CRC32

## How to use

(!) The utility only works with files with the extension `.bin` and calculates CRC32 with `0xFFFFFFFF` initial value.

1. Launch the shell and move to the directory with the utility `cd <path-to-utility>`
2. Run the utility, passing the name of the binary file as an argument. The binary name `bin_name` must be
   entered without extension! For example, Windows Command Prompt `cmd.exe`

   ```sh
   stm32-crc32 bin_name
   ```

3. If the program succeeds, the file size and the calculated CRC32 in HEX format are output.

## Building

If you have installed GNU build utils and want to rebuild the utility yourself, run the command

```sh
make all
```

at the root of the repository.

On Windows platform the unility was built using MinGW tools.
