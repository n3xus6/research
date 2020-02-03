# RSA-Algorithm
Famous asymmetric cryptosystem presented 1977 by **R. Rivest**, **A. Shamir** and **L. A. Adleman**.

The RSA-Algorithm is based on the fact that it is easy to create the product of two prime numbers but it is nearly impossible to factorize it back for a large enough number.

**Creating the public key and the private key**

Choose two prime numbers and multiply them to receive the product _P_:

```
P = p1 * p2
```

Next, calculate _F_ with this formula:

```
F = (p1 - 1) * (p2 -1)
```

Then choose a _t_ so it satisfies the following conditions:

```
t > 1, t < F, t and F are coprime
```

There are different strategies to choose a good value for _t_.

Solve this equation to find the value of _i_:

```
(t * i) mod F = 1
```

The public key consists of (_P_, _t_) and can be shared.

The secret key consists of (_P_, _i_) and must be kept private.


**Encryption of a plaintext n**

```
v = n^t mod P
```

The message is transformed to a number _n_ < _P_.
Using the public key everybody can create and send an encrypted message.

**Decryption of a cyphertext v**

```
n = v^i mod P
```

Only the owner of the private key can decrypt the message _v_.

**Example**

_p1_ = 13, _p2_ = 17

```
P = 13 * 17 = 221
F = (13 - 1) * (17 - 1) = 192
```

_t_ = 19

```
(19 * i) mod 192 = 1
i = 91
```

Public key: _P_ = 221, _t_ = 19

Private key: _P_ = 221, _i_ = 91

The sender encrypts the secret message _n_ = 42 with the public key from the receiver:

```
v = 42^19 mod 221 = 172
```

Only the receiver can decrypt the cyphertext _v_ = 172 using the private key on his computer:

```
n = 172^91 mod 221 = 42
```

Most of the information here is from the book [Algorithmen kompakt und verstaendlich](https://www.springer.com/de/book/9783658186104).

Notes
* To create a RSA keypair and use it in practice try [GnuPG](https://gnupg.org/).
* It is possible that a suspect preserves encrypted messages for the future when it might become possible to break the cryptosystem.
* Never ever implement cryptography your own unless you are a renowned expert in this field...
