#ifndef SEQUENCE_H
#define SEQUENCE_H

template <typename T>
class Sequence {
public:
    virtual ~Sequence() = default;

    virtual const T& Get(int index) const = 0;
    virtual T& Get(int index) = 0;
    virtual int GetLength() const = 0;
    virtual void Append(const T& item) = 0;
};

#endif