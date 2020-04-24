FROM cppchriscpp/cc65-node:latest

WORKDIR /
COPY . /
COPY tools/github-action-build/build.sh /build.sh

ENTRYPOINT ["sh", "/build.sh"]