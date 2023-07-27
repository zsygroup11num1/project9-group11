# project9-group11
常量定义：代码中定义了AES的S-Box（Substitution Box）和Rcon（Round Constants）常量。S-Box用于SubBytes步骤中的字节替换，Rcon用于密钥扩展过程。

AES SubBytes：aes_sub_bytes函数对输入的16字节数据进行S-Box字节替换。它使用预定义的S-Box表对每个字节进行替换，实现了AES中SubBytes步骤。

AES ShiftRows：aes_shift_rows函数对输入的16字节数据进行行移位。它将状态数组的每一行进行不同的循环移位操作，实现了AES中ShiftRows步骤。

AES MixColumns：aes_mix_columns函数对输入的16字节数据进行列混淆。它通过将每一列看作4个多项式，并执行有限域乘法操作，实现了AES中MixColumns步骤。

AES AddRoundKey：aes_add_round_key函数对输入的16字节数据与轮密钥进行异或操作。这是AES算法中每一轮的最后一步，它将轮密钥与状态数组进行逐字节异或。

AES Key Expansion：aes_key_expansion函数实现AES的密钥扩展算法。它根据输入的128位密钥生成轮密钥，用于每一轮的AddRoundKey操作。

AES Encrypt：aes_encrypt函数是主要的加密函数。它接受明文和密钥，然后依次执行AES算法的SubBytes、ShiftRows、MixColumns和AddRoundKey等步骤。最后输出加密后的结果。

测试主函数：在主函数中，我定义了一个AES-128密钥和明文，并输出加密后的密文。

运行结果：<img width="483" alt="070fc4d29c2c66ac7783e190c6489fb" src="https://github.com/zsygroup11num1/project9-group11/assets/129477117/7a226eea-4e3c-439c-be70-8dbc9f293b21">
