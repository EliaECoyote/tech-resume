# Reference docs: https://github.com/sgerrand/alpine-pkg-glibc#installing
# Adds glibc, in order to make reason-vscode work correctly
apk --no-cache add ca-certificates wget
wget -q -O /etc/apk/keys/sgerrand.rsa.pub https://alpine-pkgs.sgerrand.com/sgerrand.rsa.pub
wget https://github.com/sgerrand/alpine-pkg-glibc/releases/download/2.30-r0/glibc-2.30-r0.apk
apk add glibc-2.30-r0.apk
