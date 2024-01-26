FROM igwgames/cc65-node:latest

WORKDIR /
COPY . /
COPY tools/github-action-build/build.sh /build.sh

WORKDIR /tools/tmx2c/src
RUN npm install
WORKDIR /tools/sprite_def2img/src
RUN npm install
WORKDIR /tools/chr2img/src
RUN npm install

ENTRYPOINT ["sh", "/build.sh"]