#include "hw/l3_DomainLayer.h"
#include <vector>

using namespace std;

bool Candidate::invariant() const
{
    return _name.size() <= MAX_FIO_FIELD_LENGTH && _surname.size() <= MAX_FIO_FIELD_LENGTH && _age <= MAX_AGE && _age >= MIN_AGE && !_name.empty() && !_surname.empty();
}

Candidate::Candidate(const string &name, const string &surname,
                     uint age, uint income, uint voices, uint district_id)
    : _name(name), _surname(surname), _age(age), _income(income),
      _voices(voices), _district_id(district_id)
{
    assert(invariant());
}

Candidate::Candidate(const char *name, const char *surname,
                     long unsigned int age, long unsigned int income,
                     long unsigned int voices,
                     long unsigned int district_id)
{
    _name = string(name);
    _surname = string(surname);
    _age = age;
    _income = income;
    _voices = voices;
    _district_id = district_id;
    assert(invariant());
}

const string &Candidate::getName() const { return _name; }
const string &Candidate::getSurname() const { return _surname; }
const uint Candidate::getAge() const { return _age; }
const uint Candidate::getIncome() const { return _income; }
const uint Candidate::getVoices() const { return _voices; }
const uint Candidate::getDistrictId() const { return _district_id; }

const string &PoliticalFraction::getName() const { return _name; }
const vector<Candidate> PoliticalFraction::getCandidates() const { return _candidates; }

bool PoliticalFraction::invariant() const
{
    return (_name.size() > 0) && (_name.size() < MAX_FRACTION_LENGTH);
}

PoliticalFraction::PoliticalFraction(const string &name) : _name(name)
{
    assert(invariant());
}
PoliticalFraction::PoliticalFraction(const char *name)
{
    _name = string(name);
    assert(invariant());
}

void PoliticalFraction::addCandidate(const string &name, const string &surname,
                                     uint age, uint income, uint voices, uint district_id)
{
    _candidates.push_back(Candidate(name, surname,
                                    age, income, voices, district_id));
}

void PoliticalFraction::addCandidate(const char *name, const char *surname, long unsigned int age,
                                     long unsigned int income, long unsigned int voices,
                                     long unsigned int district_id)
{
    _candidates.push_back(Candidate(name, surname,
                                    age, income, voices, district_id));
}

bool PoliticalFraction::write(ostream &os)
{
    writeString(os, _name);

    size_t n_candidates = _candidates.size();
    writeNumber(os, n_candidates);

    for (auto &candidate : _candidates)
    {
        writeString(os, candidate.getName());
        writeString(os, candidate.getSurname());
        writeNumber(os, candidate.getAge());
        writeNumber(os, candidate.getIncome());
        writeNumber(os, candidate.getVoices());
        writeNumber(os, candidate.getDistrictId());
    }

    return os.good();
}

shared_ptr<ICollectable> FractionsCollector::read(istream &is)
{
    string name = readString(is, MAX_FRACTION_LENGTH);
    shared_ptr<PoliticalFraction> fraction = std::make_shared<PoliticalFraction>(name);

    size_t n_candidates = readNumber<size_t>(is);
    for (size_t i = 0; i < n_candidates; ++i)
    {
        string name = readString(is, MAX_FIO_FIELD_LENGTH);
        string surname = readString(is, MAX_FIO_FIELD_LENGTH);
        uint age = readNumber<uint>(is);
        uint income = readNumber<uint>(is);
        uint voices = readNumber<uint>(is);
        uint district_id = readNumber<uint>(is);
        fraction->addCandidate(name, surname, age, income, voices, district_id);
    }

    return fraction;
}

// bool Candidate::write(ostream &os)
// {
//     writeString(os, _name);
//     writeString(os, _surname);
//     writeNumber(os, _age);
//     writeNumber(os, _income);
//     writeNumber(os, _voices);
//     writeNumber(os, _district_id);

//     return os.good();
// }

// shared_ptr<ICollectable> ItemCollector::read(istream &is)
// {
//     string name = readString(is, MAX_FIO_FIELD_LENGTH);
//     string surname = readString(is, MAX_FIO_FIELD_LENGTH);
//     uint age = readNumber<uint>(is);
//     uint income = readNumber<uint>(is);
//     Fraction fraction = Converter::toFraction(readString(is, MAX_FRACTION_LENGTH));
//     uint voices = readNumber<uint>(is);

//     return make_shared<Candidate>(name, surname, age, income, fraction, voices);
// }
