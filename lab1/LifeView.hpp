#pragma once

namespace life {
    class ILifeView {
    public:
        virtual int exec(int argc, char ** arg) = 0;
        virtual ~ILifeView() = default;
    };
}
