![image](https://user-images.githubusercontent.com/34389545/35821974-62e0e25c-0a70-11e8-87dd-2cfffeb6ed47.png)

#### Master Build Status
[![Build Status](https://github.com/turtlecoin/node-cryptonote-util/workflows/CI%20Build%20Tests/badge.svg?branch=master)](https://github.com/turtlecoin/node-cryptonote-util/actions)

#### Development Build Status
[![Build Status](https://github.com/turtlecoin/node-cryptonote-util/workflows/CI%20Build%20Tests/badge.svg?branch=development)](https://github.com/turtlecoin/node-cryptonote-util/actions)

[![NPM](https://nodei.co/npm/turtlecoin-cryptonote-util.png?downloads=true&stars=true)](https://nodei.co/npm/turtlecoin-cryptonote-util/)

# Node-Cryptonote-Util

**Note:** We build prebuilds of the Node.js native addon module binaries that are included for distribution with the NPM
installed version of this package to speed up your development and use of this package.

Supported on the following platforms:

* Linux 64-bit
* Windows 64-bit

## Dependencies

* Node.js LTS (https://nodejs.org/) 10+
* Boost (http://www.boost.org/)

#### Windows (if not using prebuilds)

Read very careful if you want this to work right the first time.

1) Open a *Windows Powershell* console as **Administrator**

2) Run the command: `npm install -g windows-build-tools --vs2015`
   ***This will take a while. Sit tight.***

## Installation

```bash
npm install turtlecoin-cryptonote-util
```

### Intialization

#### TypeScript

##### Importing Individual Synchronous Methods

```javascript
import { 
    address_decode, 
    construct_block_blob, 
    convert_blob, 
    convert_blob_bb, 
    get_block_id 
} from 'turtlecoin-cryptonote-util';
```

##### Importing Async Class

```javascript
import CryptoNoteUtils from 'turtlecoin-cryptonote-util';
```

#### CommonJS

```javascript
const cnUtils = require('turtlecoin-cryptonote-util');
```

### Documentation

You can find the full TypeScript/JS documentation for this library [here](https://cnutils.turtlecoin.dev).