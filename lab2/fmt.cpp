#include "fmt.hpp"

sp::AudioFile::AudioFile(const std::string & filename, const std::string & openmode) {
    if (openmode == "r")
        this->file_.open(filename, std::ios_base::binary | std::ios_base::in);
    else if (openmode == "w")
        this->file_.open(filename, std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
    else
        throw sp::FileOpeningError("wrong openmode: " + openmode + std::string("\n"));
    if (!this->file_.is_open())
        throw sp::FileOpeningError("couldn't open the file: " + filename + std::string("\n"));
}

void sp::WaveFile::read_header() {
    char chunk_id[5] = {0};
    uint32_t chunk_size = 0;
    char format[5] = {0};

    this->file_.read(chunk_id, sizeof(chunk_id) - 1);
    this->file_.read(reinterpret_cast<char *>(&chunk_size), sizeof(chunk_size));
    this->file_.read(format, sizeof(format) - 1);

    if (std::string(chunk_id) != "RIFF" || std::string(format) != "WAVE")
        throw sp::FileFormatError("it's not a correct WAVE-file\n");

    char subchunk_id[5] = {0};
    uint32_t subchunk_size = 0;

    this->file_.read(subchunk_id, sizeof(subchunk_id) - 1);
    this->file_.read(reinterpret_cast<char *>(&subchunk_size), sizeof(subchunk_size));
    while (std::string(subchunk_id) != "fmt " && !this->file_.eof()) {
        this->file_.seekg(this->file_.tellg() + static_cast<std::streamoff>(subchunk_size));
        this->file_.read(subchunk_id, sizeof(subchunk_id) - 1);
        this->file_.read(reinterpret_cast<char *>(&subchunk_size), sizeof(subchunk_size));
    }

    if (std::string(subchunk_id) != "fmt ")
        throw sp::FileFormatError("it's not a correct WAVE-file\n");

    this->file_.read(reinterpret_cast<char *>(&(this->audio_format_)), sizeof(this->audio_format_));
    this->file_.read(reinterpret_cast<char *>(&(this->num_channels_)), sizeof(this->num_channels_));
    this->file_.read(reinterpret_cast<char *>(&(this->sample_rate_)), sizeof(this->sample_rate_));
    this->file_.read(reinterpret_cast<char *>(&(this->byte_rate_)), sizeof(this->byte_rate_));
    this->file_.read(reinterpret_cast<char *>(&(this->block_align_)), sizeof(this->block_align_));
    this->file_.read(reinterpret_cast<char *>(&(this->bits_per_sample_)), sizeof(this->bits_per_sample_));

    if (!this->fmt_is_supported())
        throw sp::FileFormatError("it's an unsupported WAVE-file format\n");

    this->file_.read(subchunk_id, sizeof(subchunk_id) - 1);
    this->file_.read(reinterpret_cast<char *>(&subchunk_size), sizeof(subchunk_size));
    while (std::string(subchunk_id) != "data" && !this->file_.eof()) {
        this->file_.seekg(this->file_.tellg() + static_cast<std::streamoff>(subchunk_size));
        this->file_.read(subchunk_id, sizeof(subchunk_id) - 1);
        this->file_.read(reinterpret_cast<char *>(&subchunk_size), sizeof(subchunk_size));
    }
    if (std::string(subchunk_id) != "data")
        throw sp::FileFormatError("it's not a correct WAVE-file\n");
}

bool sp::WaveFile::fmt_is_supported() {
    switch (this->audio_format_) {
        case sp::formats::WAVE_FORMAT_PCM: break;
        default: return false;
    }
    switch (this->num_channels_) {
        case sp::channels::MONO: break;
        default: return false;
    }
    switch (this->sample_rate_) {
        case sp::rates::SAMPLE_RATE_44100: break;
        default: return false;
    }
    switch (this->bits_per_sample_) {
        case sp::types::SIGNED_INT_16: break;
        default: return false;
    }
    if (this->audio_format_ == sp::formats::WAVE_FORMAT_PCM) {
        if (this->block_align_ != this->num_channels_ * this->bits_per_sample_ / CHAR_BIT)
            return false;
        if (this->byte_rate_ != this->sample_rate_ * this->block_align_)
            return false;
    }
    return true;
}
