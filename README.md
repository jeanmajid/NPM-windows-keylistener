# @jeanmajid/windows-keylistener

Low‑level global keyboard hook for Windows. Very basic

## Installation

```sh
npm install @jeanmajid/windows-keylistener
```

## Usage

CommonJS (Node.js):

```javascript
const keylistener = require('@jeanmajid/windows-keylistener');

keylistener.start((keyCode) => {
    console.log('Key pressed:', keyCode);
});

// later...
keylistener.stop();

// copy a string to clipboard:
keylistener.copyToClipboard('Hello from clipboard');
```

ESM:

```javascript
import keylistener from '@jeanmajid/windows-keylistener';

keylistener.start((keyCode) => {
  console.log('Key pressed:', keyCode);
});
```

## API

- `start`  
  Starts listening; takes a callback `(keyCode: number) => void`.

- `stop`  
  Stops listening and clears the callback.

- `copyToClipboard`  
  Copies a given string into the Windows clipboard.

Type definitions are in keylistener.d.ts.

## License

MIT © 2025 jeanmajid