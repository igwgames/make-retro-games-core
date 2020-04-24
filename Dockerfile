FROM cppchriscpp/cc65-node:latest

COPY . /
COPY tools/github-action-build/build.sh /build.sh

ENTRYPOINT ["/build.sh"]