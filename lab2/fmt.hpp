#pragma once

#include <exception>
#include <cstdint>
#include <climits>
#include <fstream>
#include <string>

namespace sp {
    class FileError : public std::exception {
    public:
        FileError(const std::string & msg) : _msg(msg) {}
        virtual const char * what() const noexcept { return _msg.c_str(); }
    private:
        std::string _msg;
    };

    class FileOpeningError : public FileError {
    public:
        FileOpeningError(const std::string & msg) : FileError(msg) {}
    };

    class FileFormatError : public FileError {
    public:
        FileFormatError(const std::string & msg) : FileError(msg) {}
    };
}

namespace sp {
    class AudioFile {
    public:
        AudioFile(const std::string & filename, const std::string & openmode);
        virtual ~AudioFile() {}
        virtual void read_header() = 0;
        virtual bool fmt_is_supported() = 0;
    protected:
        std::fstream file_;
    };

    class WaveFile : public AudioFile {
    public:
        WaveFile(const std::string & filename, const std::string & openmode) : AudioFile(filename, openmode) {
            this->read_header();
        }
    private:
        void read_header();
        bool fmt_is_supported();

        uint16_t audio_format_;
        uint16_t num_channels_;
        uint32_t sample_rate_;
        uint32_t byte_rate_;
        uint16_t block_align_;
        uint16_t bits_per_sample_;
    };
}

namespace sp {
    namespace formats {
        const uint16_t WAVE_FORMAT_PCM = 0x0001;
    }
    namespace rates {
        const uint32_t SAMPLE_RATE_44100 = 44100;
    }
    namespace channels {
        const uint16_t MONO = 1;
    }
    namespace types {
        const uint16_t SIGNED_INT_16 = 16;
    }
}
