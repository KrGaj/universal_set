#ifndef UNIVERSAL_SET_HPP
#define UNIVERSAL_SET_HPP

#include <iostream>
#include <array>
#include <bitset>
#include <utility>
#include <exception>

template <typename E, std::size_t UNIVERSE_SIZE>
class UniversalSet {
    public:
    class Vector {
        public:
        class Iterator {
            private:
            Iterator(const Vector* v_ptr, const std::size_t idx=0u);

            public:
            Iterator(const Iterator& rhs) = default;
            Iterator& operator=(const Iterator& rhs) = default;
            Iterator(Iterator&& rhs) = default;
            Iterator& operator=(Iterator&& rhs) = default;

            friend class Vector;

            private:
            const Vector *vector_ptr;
            std::size_t current_index;
            const E nullElem{E()};  //stały element o wartości domyślnej, zwracany przez iteratory "end"

            public:
            bool operator==(const Iterator& rhs) const;
            bool operator!=(const Iterator& rhs) const;
            // Przechodzi na kolejny (istniejacy!) element w "Vector"
            Iterator& operator++();
            Iterator operator++(int);
            const E& operator*() const;
            const E* operator->() const;
            operator bool() const;
        };

        public:
        Vector(const UniversalSet<E, UNIVERSE_SIZE>* universe_ptr);
        Vector(const UniversalSet<E, UNIVERSE_SIZE>* universe_ptr, const std::bitset<UNIVERSE_SIZE> &bs);
        Vector(const Vector& rhs) = default;
        Vector& operator=(const Vector& rhs) = default;
        Vector(Vector&& rhs) = default;
        Vector& operator=(Vector&& rhs) = default;

        friend class Iterator;

        private:
        const UniversalSet<E, UNIVERSE_SIZE> *universe;
        std::bitset<UNIVERSE_SIZE> bitset;

        public:
        // Zwraca liczbe elementów reprezentowanych przez "Vector"
        std::size_t count() const;
        // Jezeli element o indeksie "i" nie nalezy do "Vector" to dodaje ten element oraz zwraca "Iterator" do dodanego elementu i "true", w przeciwnym razie zwraca samo co "end()" i "false"
        std::pair<Iterator, bool> insert(std::size_t idx);
        // Jezeli element o indeksie "i" nalezy do "Vector" to zwraca "true", w przeciwnym razie zwraca "false"
        bool isMember(std::size_t idx) const;
        // Jeżeli element o indeksie "i" należy do "Vector" to zwraca "Iterator" do tego elementu, w przeciwnym razie zwraca to samo co "end()"
        Iterator elemIterator(std::size_t idx) const;
        // Jezeli element o indeksie "i" nalezy do "Vector" to usuwa ten element i zwraca "true", w przeciwnym razie zwraca "false"
        bool remove(std::size_t idx);
        // Zwraca "true" jeżeli obiekt "v2" reprezentuje ten sam zestaw elementow z tego samego uniwersum, w przeciwnym razie zwraca "false"
        bool operator==(const Vector& rhs) const;
        bool operator!=(const Vector& rhs) const;
        // Zwraca sume zbiorow
        Vector operator+(const Vector& rhs) const;
        // Zwraca roznice zbiorow
        Vector operator-(const Vector& rhs) const;
        // Zwraca czesc wspolna zbiorow
        Vector operator*(const Vector& rhs) const;
        // Zwraca roznice symetryczna zbiorow
        Vector operator/(const Vector& rhs) const;
        // Zwraca "Iterator" na poczatek
        Iterator begin() const;
        // Zwraca "Iterator" za koniec
        Iterator end() const;
    };

    public:
    UniversalSet(const std::array<E, UNIVERSE_SIZE>& elems);
    UniversalSet(const UniversalSet& rhs) = default;
    UniversalSet& operator=(const UniversalSet& rhs) = default;
    UniversalSet(UniversalSet&& rhs) = default;
    UniversalSet& operator=(UniversalSet&& rhs) = default;

    private:
    std::array<E, UNIVERSE_SIZE> elements;

    public:
    constexpr std::size_t universeSize() const { return UNIVERSE_SIZE; }
    const E& elem(std::size_t idx) const;
    const E& operator[](std::size_t idx) const;
    // Zwraca pusty "Vector"
    Vector makeVector() const;
};

/****************************************************************************************/


/*----->    IMPLEMENTACJA UNIVERSALSET  <-----*/


template <typename E, std::size_t UNIVERSE_SIZE>
UniversalSet<E, UNIVERSE_SIZE>::UniversalSet(const std::array<E, UNIVERSE_SIZE>& elems) : elements(elems) {
    
}

template <typename E, std::size_t UNIVERSE_SIZE>
const E& UniversalSet<E, UNIVERSE_SIZE>::elem(std::size_t idx) const {
    if(idx>=std::size(elements)) {
        throw std::out_of_range("Wrong index");
    }

    return elements[idx];
}

template <typename E, std::size_t UNIVERSE_SIZE>
const E& UniversalSet<E, UNIVERSE_SIZE>::operator[](std::size_t idx) const {
    return elements[idx];
}

template <typename E, std::size_t UNIVERSE_SIZE>
typename UniversalSet<E, UNIVERSE_SIZE>::Vector UniversalSet<E, UNIVERSE_SIZE>::makeVector() const {
    return Vector(this);
}


/*----->    IMPLEMENTACJA VECTORA   <-----*/


template <typename E, std::size_t UNIVERSE_SIZE>
UniversalSet<E, UNIVERSE_SIZE>::Vector::Vector(const UniversalSet<E, UNIVERSE_SIZE>* universe_ptr) : universe(universe_ptr) {

}

template <typename E, std::size_t UNIVERSE_SIZE>
UniversalSet<E, UNIVERSE_SIZE>::Vector::Vector(const UniversalSet<E, UNIVERSE_SIZE>* universe_ptr, const std::bitset<UNIVERSE_SIZE>& bs)
    : universe(universe_ptr), bitset(bs) {

}

template <typename E, std::size_t UNIVERSE_SIZE>
std::size_t UniversalSet<E, UNIVERSE_SIZE>::Vector::count() const {
    //return std::size(bitset);
    return bitset.count();
}

template <typename E, std::size_t UNIVERSE_SIZE>
bool UniversalSet<E, UNIVERSE_SIZE>::Vector::isMember(std::size_t idx) const {
    return idx<std::size(bitset) ? bitset[idx] : false;
}

template <typename E, std::size_t UNIVERSE_SIZE>
bool UniversalSet<E, UNIVERSE_SIZE>::Vector::remove(std::size_t idx) {
    if(idx>=std::size(bitset)) {
        return false;
    }

    if(bitset[idx]==false) {
        return false;
    }

    bitset[idx]=false;

    return true;
}

template <typename E, std::size_t UNIVERSE_SIZE>
bool UniversalSet<E, UNIVERSE_SIZE>::Vector::operator==(const Vector& rhs) const {
    return this->universe==rhs.universe && this->bitset==rhs.bitset;
}

template <typename E, std::size_t UNIVERSE_SIZE>
bool UniversalSet<E, UNIVERSE_SIZE>::Vector::operator!=(const Vector& rhs) const {
    return !(*this==rhs);
}

template <typename E, std::size_t UNIVERSE_SIZE>
typename UniversalSet<E, UNIVERSE_SIZE>::Vector UniversalSet<E, UNIVERSE_SIZE>::Vector::operator+(const Vector& rhs) const {
    return Vector(this->universe, this->bitset | rhs.bitset);
}

template <typename E, std::size_t UNIVERSE_SIZE>
typename UniversalSet<E, UNIVERSE_SIZE>::Vector UniversalSet<E, UNIVERSE_SIZE>::Vector::operator-(const Vector& rhs) const {
    return Vector(this->universe, this->bitset & (~rhs.bitset));
}

template <typename E, std::size_t UNIVERSE_SIZE>
typename UniversalSet<E, UNIVERSE_SIZE>::Vector UniversalSet<E, UNIVERSE_SIZE>::Vector::operator*(const Vector& rhs) const {
    return Vector(this->universe, this->bitset & rhs.bitset); 
}

template <typename E, std::size_t UNIVERSE_SIZE>
typename UniversalSet<E, UNIVERSE_SIZE>::Vector UniversalSet<E, UNIVERSE_SIZE>::Vector::operator/(const Vector& rhs) const {
    return Vector(this->universe, this->bitset ^ rhs.bitset);
}

template <typename E, std::size_t UNIVERSE_SIZE>
std::pair<typename UniversalSet<E, UNIVERSE_SIZE>::Vector::Iterator, bool> UniversalSet<E, UNIVERSE_SIZE>::Vector::insert(std::size_t idx) {
    if(idx>=std::size(bitset)) {
        return {end(), false};
    }

    if(bitset[idx]==true) {
        return {end(), false};
    }

    bitset[idx]=true;

    return {Iterator(this, idx), true};
}

template <typename E, std::size_t UNIVERSE_SIZE>
typename UniversalSet<E, UNIVERSE_SIZE>::Vector::Iterator UniversalSet<E, UNIVERSE_SIZE>::Vector::elemIterator(std::size_t idx) const {
    if(idx>=std::size(bitset) || bitset[idx]==false) {
        return end();
    }

    /*if(bitset[idx]==false) {    //w chwili obecnej nie widzę lepszego rozwiązania...
        return end();
    }*/

    return Iterator(this, idx);
}

template <typename E, std::size_t UNIVERSE_SIZE>
typename UniversalSet<E, UNIVERSE_SIZE>::Vector::Iterator UniversalSet<E, UNIVERSE_SIZE>::Vector::begin() const {
    std::size_t temp_idx=0u;

    while(bitset[temp_idx]!=true && temp_idx<std::size(bitset)) {
        ++temp_idx;
    }

    return Iterator(this, temp_idx);
}

template <typename E, std::size_t UNIVERSE_SIZE>
typename UniversalSet<E, UNIVERSE_SIZE>::Vector::Iterator UniversalSet<E, UNIVERSE_SIZE>::Vector::end() const {
    return Iterator(this, std::size(bitset));
}



/*----->    IMPLEMENTACJA ITERATORA     <-----*/


template <typename E, std::size_t UNIVERSE_SIZE>
UniversalSet<E, UNIVERSE_SIZE>::Vector::Iterator::Iterator(const Vector* v_ptr, const std::size_t idx)
    : vector_ptr(v_ptr), current_index(idx) {

}

template <typename E, std::size_t UNIVERSE_SIZE>
bool UniversalSet<E, UNIVERSE_SIZE>::Vector::Iterator::operator==(const Iterator& rhs) const {
    return this->vector_ptr==rhs.vector_ptr && this->current_index==rhs.current_index;
}

template <typename E, std::size_t UNIVERSE_SIZE>
bool UniversalSet<E, UNIVERSE_SIZE>::Vector::Iterator::operator!=(const Iterator& rhs) const {
    return !(*this==rhs);
}

template <typename E, std::size_t UNIVERSE_SIZE>    //preinkrementacja
typename UniversalSet<E, UNIVERSE_SIZE>::Vector::Iterator& UniversalSet<E, UNIVERSE_SIZE>::Vector::Iterator::operator++() {
    do {
        ++current_index;
    } while(current_index<std::size(vector_ptr->bitset) && vector_ptr->bitset[current_index]!=true);

    return *this;
}

template <typename E, std::size_t UNIVERSE_SIZE>    //postinkrementacja
typename UniversalSet<E, UNIVERSE_SIZE>::Vector::Iterator UniversalSet<E, UNIVERSE_SIZE>::Vector::Iterator::operator++(int) {
    auto old=(*this);
    ++(*this);

    return old;
}

template <typename E, std::size_t UNIVERSE_SIZE>    //referencja do elementu zbioru
const E& UniversalSet<E, UNIVERSE_SIZE>::Vector::Iterator::operator*() const {
    return current_index==std::size(vector_ptr->bitset) ? nullElem : vector_ptr->universe->elements[current_index];
}

template <typename E, std::size_t UNIVERSE_SIZE>    //wskaźnik na element zbioru
const E* UniversalSet<E, UNIVERSE_SIZE>::Vector::Iterator::operator->() const {
    return current_index==std::size(vector_ptr->bitset) ? &nullElem : &(vector_ptr->universe->elements[current_index]);
}

template <typename E, std::size_t UNIVERSE_SIZE>
UniversalSet<E, UNIVERSE_SIZE>::Vector::Iterator::operator bool() const {   //czy iterator wciąż wskazuje na element zbioru
    return current_index!=std::size(vector_ptr->bitset);
}

#endif