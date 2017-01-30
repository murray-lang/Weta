/**
 * Created by murray on 28/12/14.
 */

function Token (value, offset, match)
{
    this.value = value;
    if (offset !== undefined)
        this.offset = offset;
    else
        this.offset = -1;

    if (match !== undefined)
        this.match = match;
    else
        this.match = value;
}

module.exports = Token;