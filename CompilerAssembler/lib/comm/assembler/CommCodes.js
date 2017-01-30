ByteCodes = require("../../../assembler/generate/codes/ByteCodes");
baseCodes = require("../../../assembler/generate/codes/BaseCodes");

var commCodes = new ByteCodes(0, "comm", baseCodes);
commCodes.initData =
    [
        { asm: "serial.config", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.config", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.config.ip", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.config.dns", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.config.gateway", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.config.mask", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.scan", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.ssid", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.rssi", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.bssid", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.encrypt", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.connect", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.disconnect", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.mac", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.ip", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.mask", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.gateway", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.status", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.client.connect", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.client.disconnect", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.client.connected", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.client.available", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.client.read", getCode: function (types)
            {
                return types.getNextCode();
            }
        },

        { asm: "wifi.client.write", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.server.start", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.server.stop", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.server.listen", getCode: function (types)
            {
                return types.getNextCode();
            }
        },

        { asm: "wifi.server.write", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "wifi.server.status", getCode: function (types)
            {
                return types.getNextCode();
            }
        }

    ];

module.exports = commCodes;