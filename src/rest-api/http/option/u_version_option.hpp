#ifndef HTTP_U_VERSION_OPTION_HPP
#define HTTP_U_VERSION_OPTION_HPP

namespace uhttp {

enum class Version {
    HTTP_1_0,
    HTTP_1_1
};

template<typename Request>
class VersionOption {
public:
    VersionOption(Version ver) : _version(ver) {
    }

    void SetOption(Request& req) {
        req.version(_version == Version::HTTP_1_0 ? 10 : 11);
    }

private:
    Version _version;
};

}

#endif
