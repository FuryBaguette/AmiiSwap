FROM debian:stable
   
ENV DEVKITPRO=/opt/devkitpro \
    DEVKITARM=${DEVKITPRO}/devkitARM \
    DEVKITPPC=${DEVKITPRO}/devkitPPC \
    PATH=${DEVKITPRO}/tools/bin:$PATH   

RUN apt-get update \
 && apt-get install -y curl gpg libxml2 xz-utils make git \
 && curl -sSL https://github.com/devkitPro/pacman/releases/download/devkitpro-pacman-1.0.1/devkitpro-pacman.deb -o devkitpro-pacman.deb \
 && apt install -y ./devkitpro-pacman.deb \
  ; /var/lib/dpkg/info/devkitpro-pacman.postinst \
 && rm devkitpro-pacman.deb \
 && dkp-pacman -S --noconfirm \
    switch-dev switch-sdl2 switch-sdl2_ttf \
    switch-sdl2_image switch-sdl2_gfx switch-mesa \
    switch-glad switch-glm switch-libdrm_nouveau \
    switch-libwebp switch-libpng switch-freetype \
    switch-bzip2 switch-libjpeg-turbo switch-sdl2_mixer

# Uncomment COPY to build current working directory
# COPY . /build

# Uncomment RUN git clone to fetch the repo
RUN git clone --recurse https://github.com/FuryBaguette/AmiiSwap.git /build

WORKDIR /build

# Change accordingly to build a specific branch, tag or revision
RUN git checkout master \
 && sed -i.old '62s/-lnx/-lnx -lopusfile -lopus -lwebp/' /build/Makefile \
 && make