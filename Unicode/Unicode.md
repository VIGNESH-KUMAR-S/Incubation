# UNICODE
- Unicode is a Character Set. It is a superset for all other character sets.
- Its aim is to provide a unique number to identify every character for every language, on any platform.
- **Unicode is a universal character encoding standard that assigns a code to every character and symbol in every language in the world.**
- Unicode maps every character to a specific code, called **code point**. 
- A code point takes the form of U+<hex-code> , ranging from **`U+0000 to U+10FFFF`**.
- UTF-16 little-endian is the encoding standard at Microsoft (and in the Windows operating system) for wide character.
- Yet with surrogate pairs it supports 32-bit as well for wide character.
- Windows represents Unicode characters using UTF-16 encoding, in which each character is encoded as a 16-bit value.
- `utf-8, utf-16 and utf-32 are the most used character encodings in UNICODE`.

## Examples:
```c
    wchar.h
        wchar_t     =>  Wide character set (16 bits in windows)
```
|Name|Notes|
|---|---|
|wint_t btowc(int c);   |returns the result after converting c into its wide character equivalent and on error returns WEOF.|
|int wctob(wint_t c);|returns the one byte or multibyte equivalent of c and on error returns WEOF.|
       
```c
    uchar.h
        char8_t     =>  8 bit character
        char16_t    =>  16 bit character
        char32_t    =>  32 bit character 
```

```c
    stdlib.h
```
|Name|Notes|
|---|---|
|int **mbtowc**(wchar_t *wchar, const char *mbchar, size_t count);|Convert a multibyte character to a corresponding wide character.|
|size_t **mbstowcs**( wchar_t *wcstr, const char *mbstr, size_t count);|Converts a sequence of multibyte characters to a corresponding sequence of wide characters.|
|int **wctomb**(char *mbchar, wchar_t wchar);|Convert a wide character to the corresponding multibyte character.|
|size_t **wcstombs**(char *mbstr, const wchar_t *wcstr, size_t count);|Converts a sequence of wide characters to a corresponding sequence of multibyte characters.|

```c
    mbstring.h
```
|Name|Notes|
|---|---|
|unsigned int **_mbbtombc**(unsigned int c);|Converts a single-byte multibyte character to a corresponding double-byte multibyte character.|
|unsigned int **_mbctombb**(unsigned int c);|Converts a double-byte multibyte character to a corresponding single-byte multibyte character.|

### Reference
- [Unicode](https://flaviocopes.com/unicode/)
- [Character Sets](https://www.tenouk.com/ModuleG.html)
- [Multi-byte characters and Wide Characters](https://docs.oracle.com/cd/E19205-01/819-5265/bjaks/index.html)
- [wchar_t, char16_t, char32_t, char](https://ducmanhphan.github.io/2018-12-06-How-to-use-char-wchar_t-char16_t-char32_t-right-a-way-in-C++/)