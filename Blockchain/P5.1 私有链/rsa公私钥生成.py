import rsa
(pubkey,prikey) = rsa.newkeys(512)
print(pubkey.save_pkcs1().decode())
print(prikey.save_pkcs1().decode())
