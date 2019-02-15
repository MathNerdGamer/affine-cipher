# Affine Cipher

A header-only basic Affine Cipher implementation modulo 97 (all characters commonly found on a QWERTY keyboard, including a-z, A-Z, 0-9, all symbols, tab, and newline). Since 97 is prime, Z/97 is a field, which is convenient.

Uses my header-only implementations of [integers modulo N](https://gitlab.com/mathnerd/integers-modulo-n).

# Usage
Here's a basic example:
```
#include <iostream>
#include "affine_cipher.h"

using namespace math_nerd;

int main()
{
    affine_cipher::affine_key key{ 7, 2 };

    std::string hello = "Hello, world!\n";

    std::string enc = affine_cipher::encrypt(key, hello);
    std::string dec = affine_cipher::decrypt(key,   enc);

    std::cout << "Plaintext: "  << dec << '\n';
    std::cout << "Ciphertext: " << enc << '\n';

    std::cout << "Key: (" << key.first << ',' << key.second << ")\n";

    return EXIT_SUCCESS;
}

```

Output:
```
Plaintext: Hello, world!
Ciphertext: zS@@"?wv"M@L_`
Key: (7,2)
```