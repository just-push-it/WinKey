WinKey
======

WinKey is designed to do one thing: send native keyboard inputs to Windows. 

I was developing a very niche Electron application and robotjs was not yet updated to support Node 12 / Electron 5. It will not work on anything except Windows, and I have no plans to add support for other platforms in the future.

## Usage

> const winkey = require("winkey");
> winkey.pressKeys(string);

You may send one character at a time, or a whole string.
There are special keys which require the use of specific codes:

| Physical Key | Special Code |
|--------------|--------------|
| Enter/Return | @@return     |
| Backspace    | @@backspace  |
| Control      | @@control    |
| Space Bar    | @@space      |
| Delete       | @@delete     |
| Tab          | @@tab        |

These codes must be sent by themselves, and not in the middle of another string.

>winkey.pressKeys("@@backspace");

Other special keys may be added in the future as they are fairly trivial to implement. If you need one added, feel free to submit a Pull Request or create a Feature Request under Issues.
