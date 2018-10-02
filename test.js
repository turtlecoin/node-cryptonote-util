'use strict'

const cnUtil = require('./')
const Buffer = require('safe-buffer').Buffer
const assert = require('assert')

var validAddressPrefix = 3914525
var addressPrefix = cnUtil.address_decode(new Buffer('TRTLuxN6FVALYxeAEKhtWDYNS9Vd9dHVp3QHwjKbo76ggQKgUfVjQp8iPypECCy3MwZVyu89k1fWE2Ji6EKedbrqECHHWouZN6g'))
console.log("Address Prefix: ", addressPrefix)

assert.deepEqual(validAddressPrefix, addressPrefix)