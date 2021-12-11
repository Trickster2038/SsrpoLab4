#ifndef HW_L3_DOMAIN_LAYER_H
#define HW_L3_DOMAIN_LAYER_H

#include "hw/l4_InfrastructureLayer.h"
#include <map>
#include <vector>

const size_t MAX_FIO_FIELD_LENGTH = 30;
const size_t MAX_FRACTION_LENGTH = 20;
const uint MIN_AGE = 21;
const uint MAX_AGE = 120;

class Candidate //: public ICollectable
{
    std::string _name;
    std::string _surname;
    uint _age;
    uint _income;
    uint _fraction_id;
    uint _voices;
    uint _district_id;

protected:
    bool invariant() const;

public:
    Candidate() = delete;
    // Candidate(const Candidate &p) = delete;

    // Candidate &operator=(const Candidate &p) = delete;
    Candidate(const std::string &name, const std::string &surname,
              uint age, uint income, uint voices, uint district_id);
    Candidate(const char *name, const char *surname, long unsigned int age,
              long unsigned int income, long unsigned int voices,
              long unsigned int district_id);

    const std::string &getName() const;
    const std::string &getSurname() const;
    const uint getAge() const;
    const uint getIncome() const;
    const uint getVoices() const;
    const uint getDistrictId() const;

    // virtual bool write(std::ostream &os) override;
};

class PoliticalFraction : public ICollectable
{
    std::string _name;
    std::vector<Candidate> _candidates;

protected:
    bool invariant() const;

public:
    PoliticalFraction(const std::string &name);
    PoliticalFraction(const char *name);
    PoliticalFraction() = delete;
    PoliticalFraction(const PoliticalFraction &p) = delete;
    PoliticalFraction &operator=(const PoliticalFraction &p) = delete;

    const std::string &getName() const;
    const std::vector<Candidate> getCandidates() const;
    //void addCandidate(Candidate candidate);
    void addCandidate(const std::string &name, const std::string &surname,
                      uint age, uint income, uint voices, uint district_id);
    void addCandidate(const char *name, const char *surname, long unsigned int age,
                      long unsigned int income, long unsigned int voices,
                      long unsigned int district_id);
    virtual bool write(std::ostream &os) override;
};



// class ItemCollector : public ACollector
// {
// public:
//     virtual std::shared_ptr<ICollectable> read(std::istream &is) override;
// };

class FractionsCollector : public ACollector
{
public:
    virtual std::shared_ptr<ICollectable> read(std::istream &is) override;
};

#endif // HW_L3_DOMAIN_LAYER_H
