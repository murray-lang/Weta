ByteCodes = require("../../../assembler/generate/codes/ByteCodes");
baseCodes = require("../../../assembler/generate/codes/BaseCodes");

var ioCodes = new ByteCodes(0, "io", baseCodes);
ioCodes.initData =
    [
        { asm: "din.config", getCode: function (types)
            {
                return types.getNextCode();
            }
        },
        { asm: "dout.config", getCode: function (types)
            {
                return types.getNextCode();
            }
        }
    ];

module.exports = ioCodes;