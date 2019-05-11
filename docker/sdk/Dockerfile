FROM rack:env
ARG RACK_BRANCH
RUN \
    cmake --version \
    && git clone -v --single-branch -b ${RACK_BRANCH} https://github.com/VCVRack/Rack.git \
    && cd Rack \
    && git submodule update --init --recursive \
    && make dep \
    && make
CMD bash
