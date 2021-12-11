#include "hw/l2_ApplicationLayer.h"
#include <algorithm>
#include <set>

using namespace std;

bool Application::performCommand(const vector<string> &args)
{
    if (args.empty())
        return false;

    if (args[0] == "l" || args[0] == "load")
    {
        string filename = (args.size() == 1) ? "hw.data" : args[1];

        if (!_col.loadCollection(filename))
        {
            _out.Output("Ошибка при загрузке файла '" + filename + "'");
            return false;
        }

        return true;
    }

    if (args[0] == "s" || args[0] == "save")
    {
        string filename = (args.size() == 1) ? "hw.data" : args[1];

        if (!_col.saveCollection(filename))
        {
            _out.Output("Ошибка при сохранении файла '" + filename + "'");
            return false;
        }

        return true;
    }

    if (args[0] == "c" || args[0] == "clean")
    {
        if (args.size() != 1)
        {
            _out.Output("Некорректное количество аргументов команды clean");
            return false;
        }

        _col.clean();

        return true;
    }

    if (args[0] == "a" || args[0] == "add")
    {
        if (args.size() != 2)
        {
            _out.Output("Некорректное количество аргументов команды add");
            return false;
        }
        _col.addItem(make_shared<PoliticalFraction>(args[1].c_str()));
        return true;
    }

    if (args[0] == "r" || args[0] == "remove")
    {
        if (args.size() != 2)
        {
            _out.Output("Некорректное количество аргументов команды remove");
            return false;
        }

        _col.removeItem(stoul(args[1]));
        return true;
    }

    if (args[0] == "u" || args[0] == "update")
    {
        if (args.size() != 3)
        {
            _out.Output("Некорректное количество аргументов команды update");
            return false;
        }
        const PoliticalFraction &pf = static_cast<PoliticalFraction &>(*_col.getItem(stoul(args[1])));
        vector<Candidate> cands = pf.getCandidates();
        _col.updateItem(stoul(args[1]),
                        make_shared<PoliticalFraction>(args[2].c_str(), cands));
        return true;
    }

    if (args[0] == "v" || args[0] == "view")
    {
        if (args.size() != 1)
        {
            _out.Output("Некорректное количество аргументов команды view");
            return false;
        }

        size_t count = 0;
        for (size_t i = 0; i < _col.getSize(); ++i)
        {
            const PoliticalFraction &item = static_cast<PoliticalFraction &>(*_col.getItem(i));

            if (!_col.isRemoved(i))
            {
                _out.Output("[" + to_string(i) + "]" + item.getName() + " ");
                size_t cnt_items = 0;
                for (auto &cand : item.getCandidates())
                {
                    _out.Output("\n\t[" + to_string(cnt_items) + "]" +
                                " " + cand.getName() +
                                " " + cand.getSurname() +
                                "\n\tage: " + to_string(cand.getAge()) +
                                "\n\tincome: " + to_string(cand.getIncome()) +
                                "\n\tvoices: " + to_string(cand.getVoices()) +
                                "\n\tdistrick id: " + to_string(cand.getDistrictId()));
                    cnt_items++;
                }
                count++;
            }
        }

        _out.Output("Количество элементов в коллекции: " + to_string(count));

        return true;
    }

    if (args[0] == "ac" || args[0] == "addcandidate")
    {
        if (args.size() != 8)
        {
            _out.Output("Некорректное количество аргументов команды addcandidte");
            return false;
        }
        PoliticalFraction &item =
            static_cast<PoliticalFraction &>(*_col.getItem(stoul(args[1])));
        item.addCandidate(args[2].c_str(), args[3].c_str(), stoul(args[4]),
                          stoul(args[5]), stoul(args[6]), stoul(args[7]));
        return true;
    }

    if (args[0] == "rating")
    {
        set<uint> forbidden;
        for (size_t i = 1; i < args.size(); ++i)
        {
            forbidden.insert(stoul(args[i]));
        }

        struct Rating
        {
            uint voices;
            string fraction;
            Rating(uint v, string fr)
            {
                voices = v;
                fraction = fr;
            }
        };

        vector<Rating> rating;
        for (size_t i = 0; i < _col.getSize(); ++i)
        {
            const PoliticalFraction &item = static_cast<PoliticalFraction &>(*_col.getItem(i));

            if (!_col.isRemoved(i))
            {
                rating.push_back(Rating(item.countVoices(forbidden), item.getName()));
            }
        }

        sort(rating.begin(), rating.end(), [](const Rating &lhs, const Rating &rhs)
             { return lhs.voices > rhs.voices; });

        size_t cnt = 0;
        for (auto &fract : rating)
        {
            _out.Output("[" + to_string(cnt) + "]" +
                        " " + fract.fraction +
                        " - voices: " + to_string(fract.voices));
            cnt++;
        }

        return true;
    }

    _out.Output("Недопустимая команда '" + args[0] + "'");
    return false;
}