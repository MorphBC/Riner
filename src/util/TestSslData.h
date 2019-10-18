//
//

#pragma once

//generated via:
//openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 9999
//password:
//asdf
//Common Name (eg, fully qualified host name) []:
//127.0.0.1

struct TestSslData {
    constexpr static const char *certificate_pem =
R"(-----BEGIN CERTIFICATE-----
MIIEpDCCAowCCQCnTAzarfunRTANBgkqhkiG9w0BAQsFADAUMRIwEAYDVQQDDAkx
MjcuMC4wLjEwHhcNMTkxMDE2MTYwODMxWhcNNDcwMzAyMTYwODMxWjAUMRIwEAYD
VQQDDAkxMjcuMC4wLjEwggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIKAoICAQCx
obWWMa5tARv1ULf/9a/loN1y5ZURwkUeoAJjruHfne8aozOwqY0FfH0538aFuFos
YzjNxhV49YkB5QEk07rdN3cqK9RRocgIbluCSJjwLk8J31U9CyPIKkonNtOPdvar
fso2miOSZ3VYho+zT+qIL/Aia2SzuJmtYXASS8pUzxtxlSrY1uH7mulxZGBnkZrm
0hcvegkOtmQ1QftPD3t22Sh2eWc07JkgFl+6XvRQ2n3hpgIi5jwFkfMjHQWhq0LP
8sXZcCx/d5N8eihVGU0tZ+PNbWjr3xVzebNQKBYfXeE4L7D0St59NN7aqyFLr3iu
CwIDVI2pBDXSc6+2NtV7CWljGXe41OECyKt87zAktbiWyK+Md7Argb36FnqXPxfQ
vzEzrPkGN7WKWQj9BNgKez4SNdX3iAn7364rJr3XNK7cZnGBnUBCfryycHc486VO
vUUGVsqdktI1jMOSoQB9erivF2SPHYJi2TSBKKDzO/fzabRuwT5uVOeTpg/2hmgw
H0LLDhcQ0t70OEhQ3VcO78TpSYgpfEzNbeM6lNKwBEYrb/Sw/LOfsl1i3k+L6zqf
8UydBXTO5HKt4ahTWqyqWPG2p7NiWntVcdZ83JDJcuYTlyZt4eUd3lt72+oWZ1a4
UG33CxAVchyBPO+7h8GV9RswbUBxStxhVHoVBkxPGwIDAQABMA0GCSqGSIb3DQEB
CwUAA4ICAQCbDc4unCdrgLZcg7j2Rc9LPMU5srZAXYr10IBlJXU26k4vXHaOGxfD
wEauZkkDmBq+sFYAANpKoKXQN5ANsOtXXDzpV5EoIE4Q8At6xfVU9nHxiJyVOXyz
/kIKuXtXL/FsjBIxLGl0FBLq4uoNWRNJ5PiH9kiC+319iBgyhAcqhMCKPCX5oiJP
IYjwYibbZ++BnxZ5WeZFxjUW4hmzIv4HDymHmhzAKMsncpeWO61jB80IboEcy7Ck
u4SXqyqJ2aEVWOtOMKJgg+mensSVonokr2zfNMeNlHYFKQrxllW7aq7M73ss+Hwt
ybEUYc3Jh9eKdYlSSnevSL1s3ttA3g76cjzQ57LqUavUt7uKh3IDYBNK1exfnS41
zBm4CGldCpNVaiWgKvXZckpiTMqfg97hNIGuzvWQwT57gT79QYFwLhq2eZHYeLi5
OIqD53w+7ph6tZLv8SeFjLjMMMu6eMFT8GnJ3OmVwaQIn+joAlqEUGmMFZQaKu+2
djOGIO/cncZc16FHHMao47k1np32Olhbcyxxl0dEj9Na3OUT1JkHcy3zMt9PLVFO
8muue4eLK0Jzw0SCuTX8NM34FUGJ86TIzfi1vII+1gZ0mZ8zqi0bjp53/i3cdKTl
ADfdQjZoGr7UDCgDzVg/FMQbicaqrFG46A9aR33IVQmMjFclN/rC1g==
-----END CERTIFICATE-----
)";

    constexpr static const char *private_key_pem =
R"(-----BEGIN ENCRYPTED PRIVATE KEY-----
MIIJnzBJBgkqhkiG9w0BBQ0wPDAbBgkqhkiG9w0BBQwwDgQIq+Ds2ME0suACAggA
MB0GCWCGSAFlAwQBKgQQxMt9OAmSUIS4IgR1imYD1ASCCVCXzE0QI/iwzjsSKetM
tfYWO4K+u98jJdhlA/ro7XjMBEsKbg0RNAPvdtlZPJ0p2sijnbU24/HgYu3J1XE/
2lIj7tq1T2WZtiL3o20sPN6agolbjDAddjV212PhH0iDFitY1J3RnzonKXVBzE3J
f2Qcdsjf8Z1QO4t9giv8NtzoQSinQAleGg1cRWhqP0F14uUs91nDmoowNbLcl7gV
bHc6ZNWbHSEAWG6p2DwVByLu9lGsVHs7XT36yCw/DJdqDucbThd+0e4sY/iHFE7h
LAlScHJDCOu0/ta3NaBa0fspS11VrqKHJyabubLdsj4pyEmO8QItx35zxWvUxRNJ
3dgWgpwJSnl86n8ZUEUYwaw28Z/5fd/fjmcCGpsSJLzadAiB8Y7L2bgOSUnXIg2F
YHOnjNjIKytSFdvnNreW+W53yW/EvtBkEQ1yPgtNY22fPjj9/WUUVFS4a85Hk7Ta
p5wcZCzQ546K7R4JfnMNZ1yUM1H/O/A4oFszK+sgzntQ0pXW5Z35Upcy56itsliO
Je2gUn5pHTRLv4N7ipl9O1IQC180SZTAJXbfHIKpWeujpxSUagrVZHiuTr7rZqfZ
sdk2Tq/senz+7+gVKzJ6eixCFuxLB9HlUoFTJ8CGjKmn6dtrocdQtjIXgnlX9Kft
Msshe46nrdxu0ncAXRRXHkOete8SKQzVPuERVGy2u5pi0uDR7ss5GzW7F7xUgo9i
pkH0sk9d3ltt14Ifzu7XgHsScE0QOpdcioiUKlIpRT6B05ea3M/ALO91nkL+nsCn
SXQqO9IPWPNdn+J/gN+OPSV2tUXk+lyU5PeXo/9O8HBraG3GrPMSv7tK/iQmXgOZ
6J/5YeRhEr+oGJJmzGAngQ0uEMO/dYRDxbeq5qscYzIRF/Vl7WJmNznvZPoS6s6P
d5eF85sa/fvy6/aGy93XsxVJ1uF6B4cxn5e5F3/7rrytGJvtS8+UZh8EXscPId9b
P5biQYQ/zNWD+BiViGaj2giC+W69T7d0gl4wwL3/JxONWh7mibgluhR5FZCJb8f4
v/aduGqhTnWZrR679aDDduHsAjO3xwKuZRMz7rZyUOQ8VE9zVCq/LkPAYxG7qhzI
o1kt3v6JoLuE5C9owgTzSdHale5nmUPIi99v4mRVkoEKrGe4YQubOEZQakIuCEgt
wmNuI+Fwkx6e2/8sBpStxRh/0ebrFXJxmCrsAZSz/zr/+obFso1a0QiGmKefcaTg
WvI93VWecuiYoyUdeLvzRRQAXVYEEhOtX8h5CXShwn6Z1dobXPAzATMZGXB4Pyld
HpTBl2VmA1GCnJ2Ws46AWjP8hOGmQUABK9R6CsameFlpw+ohek7N3UBsrqC1nhwa
6rOll62Kdgrg4PV8wOBX7OQidbLfbpBOIJGajLZ3fYuKNGnSNEXmXZewvQWGHybg
aepi7fcu0olsgd845dX7/GQOPpbFkBpuKLUPXWVoL7CXTlaiPsAj20cjo12J5obb
YPkRpHSL0agly5IVmroyDvyV6fIkeumbgXpPZIGi35/Vcb5dVT50Uvxe799aw5kP
iJsZNhEmg/GRiHe3BeCwcCUoVTOkKyN2FqUWjzRnuzzUSaM9PnzHoiI89O065yYd
q55hfAMDp/ZAUWFEqRyTgMgRUM+zJ6mA970DPDshxraMoWrrzxqef3fize+uZMF8
hCRkYr5+Dx9xwGoWtCSm5mJIDLh6Cw1+pAr/tpELGwL4FMZvMjQ8f+WTKv8PpsRc
tNY+XL7mE84OvthERGrSHcz8zxvg6KGLPNNynYTByX6sF3BZcYhYRyAiNhLFi7f2
+STsytQgveiR5s3aYGQgFeuMFjVn3KMw1VMpGpPamEPxLHoLu6Q4R3WoPCZC+avN
MjY+cNN9rkIy6J3TZN+4BGD9F/97MHyS+XYA9xX9XJOXV0kJ/OhewCyxrjngBc+y
ezzYewE3HgDYNUe8jF9neXiA26p8qon5fXnQFUMywLYM49gHFxwbzI4eFjteEdxm
IZNuAm1NQhI6NZk/dZE9SVBNWU6YcE8Ox9FKuWshVlvHjtLAkaMpri95QZkgFT4P
+SmEOM6wvgilN4qWkl7Cau4zj/GRYxkhhFzNnobZUFT+zTuGuTAt4MuMHH3YBE4E
Ow60s4cyLp2sJ4l1ZdHuMCM93i7fs5fzrD4j8SObksbbdnVe+2bRlq3UKvw4GsNc
/tLKlm9O+jA8Gv0cySo/13VBUvHUodXhsWECype70aD7WmlotI2vt1LDuEJqzAJl
vgX+7UtKMzYxQIM2Zo7GTYrQ/0XZh5WCtZmCIRS4BZeM5y7oIwQOc1drdKnw08Lo
N1vVJxuz2QQ5PG4wKOEIoTPGkl4qhFTWEaN2/pKEhcgUmjLcS0S4inD9ryKAmorz
8NY3R0VH9J/vcmnOLUGGQUQ4TAv0514VxGXszTykTBft9JGugsmnlMYw0nmhxOgJ
32aJFNs3hirzuN03FPCi8lmdxUiWjbszVKHcoRSKGKhHe1BZQeGdIcUvzgRX3KjU
UmgEksyAxkcJ2bjHNxkIuD4pELo2446uMw0LCXAdciJKbNtaquxjqB0P799pW0DB
bLsRJQv0ij34YUDz5U95PubOMaXq3KE6V9GHb6DPLgYF8YQ028li+HEQNg7sCPDS
QkDnl2gV5wQxMyJ0zu9HVFcdLzO4krCxZTXWpkBAPdHnFWrudlSv6t1IXsZ0nKdK
KcfpBqlEnmU6xnZ+BNtXyjror+UplEZA6mFdYu+iG8IbC1c9aMjfFP5vTIBCe2KC
Szet6+vtm+dYceqIhi7mdg6IFAKXHAGQZUvLDQcxdtRpRqmtRJGrXeEO8/Ibn9v9
BJDhQ3WFc56rPk0RpjkGOEeKknqUNvTUawPOV+tJZJ1GkZxVYTFtVU6+ydm/GlZm
gpkUTlTxTsfECQiZ67qF2TqxDsagInIUKWkPT8qr+7J6+G5xhNVoJ7f+D+JqlZdE
dEoBIpNzhnJGZ/Oqd1YeSsRy2ItP+SWQzWw60TImpAwUEkChSWA03KCCGHVdM8KT
cQpGBPCMdd9cIMpExJYcHpo6MZhHwG61VOiUrRCu8kqRzLW1G4QYTb/5LqdZ+0rI
bgViD3dSLewqUzws+n3CXof3afUZGJupc64jvO1CF8sQfyIENwEwxaUdKQIDQaW9
BG54ujalpjpCAQV+DX0m5KWRHA==
-----END ENCRYPTED PRIVATE KEY-----
)";
};