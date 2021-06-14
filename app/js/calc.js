//Initialization params
let instance = null;
let memoryStates = new WeakMap();

function syscall(instance, n, args) {
    switch (n) {
        default:
            // console.log("Syscall " + n + " NYI.");
            break;
        case /* brk */
        45:
            return 0;
        case /* writev */
        146:
            return instance.exports.writev_c(args[0], args[1], args[2]);
        case /* mmap2 */
        192:
            debugger;
            const memory = instance.exports.memory;
            let memoryState = memoryStates.get(instance);
            const requested = args[1];
            if (!memoryState) {
                memoryState = {
                    object: memory,
                    currentPosition: memory.buffer.byteLength,
                };
                memoryStates.set(instance, memoryState);
            }
            let cur = memoryState.currentPosition;
            if (cur + requested > memory.buffer.byteLength) {
                const need = Math.ceil((cur + requested - memory.buffer.byteLength) / 65536);
                memory.grow(need);
            }
            memoryState.currentPosition += requested;
            return cur;
    }
}

function generate(data) {
    fetch('/app/asm/main.wasm').then(response =>
        response.arrayBuffer()
    ).then(bytes => WebAssembly.instantiate(bytes, {
        env: {
            __syscall0: function __syscall0(n) {
                return syscall(instance, n, []);
            },
            __syscall1: function __syscall1(n, a) {
                return syscall(instance, n, [a]);
            },
            __syscall2: function __syscall2(n, a, b) {
                return syscall(instance, n, [a, b]);
            },
            __syscall3: function __syscall3(n, a, b, c) {
                return syscall(instance, n, [a, b, c]);
            },
            __syscall4: function __syscall4(n, a, b, c, d) {
                return syscall(instance, n, [a, b, c, d]);
            },
            __syscall5: function __syscall5(n, a, b, c, d, e) {
                return syscall(instance, n, [a, b, c, d, e]);
            },
            __syscall6: function __syscall6(n, a, b, c, d, e, f) {
                return syscall(instance, n, [a, b, c, d, e, f]);
            },
            putc_js: function (c) {
                c = String.fromCharCode(c);
                if (c == "\n") {
                    console.log(s);
                    s = "";
                } else {
                    s += c;
                }
            }
        }
    })).then(results => {
        wasm = results.instance;

        const {memory, print} = wasm.exports;
        const encode = function stringToIntegerArray(string, array) {
            const alphabet = "0123456789ABCDEF:";
            for (let i = 0; i < string.length; i++) {
                array[i] = alphabet.indexOf(string[i]);
            }
        };

        const decode = function integerArrayToString(array) {
            const alphabet = "0123456789ABCDEF:";
            let string = "";
            for (let i = 0; i < array.length; i++) {
                string += alphabet[array[i]];
            }
            return string;
        };
		
        const plaintext = reverse(data)+ ":"+ reverse(data);
        const myArray = new Int32Array(memory.buffer, 0, plaintext.length);

        encode(plaintext, myArray);

        wasm.exports.print(myArray.byteOffset, myArray.length);
		let result = decode(myArray);
		
        document.getElementById("y").value = reverse(result.substring(result.indexOf(':')+1, result.length));
        document.getElementById("x").value = reverse(result.substring(0, result.indexOf(':')));

        document.getElementById('loader').style.display = "none";
    }).catch(console.error);

    function reverse(string) {
        let copy = '';
        for (let i = string.length-1; i > 0; i -= 2) {
            copy += string[i-1];
            copy += string[i];
        }

        return copy;
    }
}
