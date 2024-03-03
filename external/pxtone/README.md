# pxtone

This is a version of pxtone based on version 170212a.

PCM data can be generated from the song data made by pxtone collage.
This PCM data can be saved as a `.wav` or output to an audio buffer to
be used as background music for games, etc.

Includes various goodies, like CMake configs, as well as some fixes from [here](https://github.com/Wohlstand/libpxtone).

## Flow

Mainly uses the `pxtnService` class.

1. Initialization
2. File loading
3. Preparing for playback
4. PCM data generation
5. Output to audio buffer

For streaming playback, (as background music, etc.) do not sample all at once,
but repeat steps 4 and 5 to output little by little.

## License

pxtone does not have a formal license, though one could assume it is
under the public domain.

<details>
	<summary>Japanese</summary>

	<ライセンス的なこと>

	- 再生に必要な ソースコード(srcとincludeフォルダ内)は無償で使えます。改変もOKです。
	- 特に許可をとる必要はありません。
	- 利用の明記についてはお任せします。
	- 利用が原因で何か問題があった場合の責任は負いかねます。
</details>

<details>
	<summary>English</summary>

	<Licensing>

	- The source code (content of "src" and "include" folders) required for playback can be used free of charge. Modification is okay.
	- No special permission is required.
	- We leave it to you to clarify the usage.
	- We are not responsible for any problems caused by using this software.
</details>