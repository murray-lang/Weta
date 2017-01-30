baseCodes   = require("./BaseCodes");

var wvmCodes =
    {
        base:   baseCodes,

        init: function ()
                {
                        // Initialise the base library first, as its codes could
                        // be used by the other libraries.
                    this.base.init();
                    for (var libName in this)
                    {
                        if (libName != "base")  // Already done
                            this[libName].init();
                    }
                }
    };

    // Prevent the init function being enumerated as a library
Object.defineProperty(wvmCodes, "init", { enumerable: false});

module.exports = wvmCodes;
