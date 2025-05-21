# How to create TOTP

## Necessary Parameters  :

	    Key = string with 64 hex converted to 32 byte
	    Message  = Current timestamp / 30 and converted to 8 bytes

## HMAC-SHA 1 Algorithm :

 1. ### Format the key :
		If the key is less or more than 64 bytes, it is filled or shortened.
 2. ### Create 2 mask :
		We create 2 mask with xor(⊕)  ipad(0x36) / opad(0x5c). Each one applies separately to the key.
 3. ### Hash message :
		We hash the message a first time with k_ipad ⇒ inner.
		And we hash inner with k_opad ⇒ HMAC Final key with 20 bytes.

## Convert HMAC to 6 digit code :

Get the last bytes of the HMAC key.

    int offset = hmac[19] & 0x0F; // for get one octets in range {0 / 15}.

From this byte we will look for 4 bytes (Because we don't want to take the first 4 bytes).

Finally, we modulo by 1,000,000 to obtain a number between 000000 and 999999.