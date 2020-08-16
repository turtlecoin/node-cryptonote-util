![image](https://user-images.githubusercontent.com/34389545/35821974-62e0e25c-0a70-11e8-87dd-2cfffeb6ed47.png)

#### Master Build Status
[![Build Status](https://travis-ci.org/turtlecoin/node-cryptonote-util.svg?branch=master)](https://travis-ci.org/node-cryptonote-util/turtlecoin) [![Build status](https://ci.appveyor.com/api/projects/status/github/turtlecoin/node-cryptonote-util?branch=master&svg=true)](https://ci.appveyor.com/project/RocksteadyTC/node-cryptonote-util/branch/master)


#### Development Build Status
[![Build Status](https://travis-ci.org/turtlecoin/node-cryptonote-util.svg?branch=development)](https://travis-ci.org/node-cryptonote-util/turtlecoin) [![Build status](https://ci.appveyor.com/api/projects/status/github/turtlecoin/node-cryptonote-util?branch=development&svg=true)](https://ci.appveyor.com/project/RocksteadyTC/node-cryptonote-util/branch/development)

[![NPM](https://nodei.co/npm/turtlecoin-cryptonote-util.png?downloads=true&stars=true)](https://nodei.co/npm/turtlecoin-cryptonote-util/)

# Node-Cryptonote-Util

Supported on the following platforms:

* Linux 64-bit
* Windows 64-bit

## Dependencies

* Node.js LTS (https://nodejs.org/) 10+
* Boost (http://www.boost.org/)

## Installation Instructions

### *Nix

```bash
sudo apt-get install libboost-all-dev
git clone https://github.com/turtlecoin/node-cryptonote-util
cd node-cryptonote-util
npm install && npm test
```

### Windows

#### Prerequisite

Read very carefully if you want this to work right the first time.

1) Open a *Windows Powershell* console as **Administrator**
2) Run the command: `npm install -g windows-build-tools --vs2015`
   ***This will take a while. Sit tight.***
   
Once the prerequisites are complete, proceed with the following:

```bash
cd <your node-cryptonote-util directory>
npm install && npm test
```
