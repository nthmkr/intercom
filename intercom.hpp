#ifndef INTERCOM_HPP
#define INTERCOM_HPP

#include <iostream>
#include <vector>

class Intercom {
public:
    Intercom() : state (State::Symbol1)
    {
        std::cout << "This is an intercom." << std::endl;
    }

    void parsing(char in)
    {
        if ((in == kBackspace) && (buffer.size() > 0)) {
            buffer.pop_back();
            if (static_cast<uint8_t>(state) > 1) {
                state = static_cast<State>(static_cast<uint8_t>(state) - 1);
            }

            print_output();
            return;
        }

        if (is_number(in) || (in == '#') || (in == '*')) {
            switch (state) {
                // Input numbers
                case State::Symbol1 :
                case State::Symbol2 :
                case State::Symbol3 :
                case State::Symbol5 :
                case State::Symbol6 :
                case State::Symbol7 :
                case State::Symbol8 :
                {
                    if (is_number(in)){
                           buffer.push_back(in);
                           state = static_cast<State>(static_cast<uint8_t>(state) + 1);
                    }
                    break;
                }

                // Calling or entering with password
                case State::Symbol4 :
                {
                    if (in == '#') {
                        uint16_t flat_number = extract_flat_number(buffer);
                        call_apartment(flat_number);
                        buffer.clear();
                        state = State::Symbol1;

                    } else if (in == '*') {
                        buffer.push_back(in);
                        state = State::Symbol5;
                    }
                    break;
                }

                // Entering with password
                case State::Symbol9 :
                {
                    if (in == '#') {
                        uint16_t flat_number = extract_flat_number(buffer);
                        if (flat_is_real(flat_number)) {
                            check_password(buffer);
                        } else {
                            std::cout << "No apartment with this number!" << std::endl;
                        }
                        buffer.clear();
                        state = State::Symbol1;
                    }
                    break;
                }
            }

            print_output();

        } else { /* It wasn't a number or special character. */
            std::cout << "Not allowed symbol!" << std::endl;
        }
    }

private:
    void print_output() {
        if (buffer.size() != 0) {
            for (auto k: buffer) {
                std::cout << k;
            }
            std::cout << std::endl;
        }
    }

    void check_password(const std::vector<char>& buf)
    {
        bool result{true};
        if (buf.size() < 8) {
            std::cout << "Not enough data!" << std::endl;
            return;
        }

        for (auto i = 0; i < kPasswordLength; ++i) {
            if (buf.at(4 + i) != password[i]) {
                result = false;
                break;
            }
        }

        if (result) {
            std::cout << "Please enter!" << std::endl;
        } else {
            std::cout << "Incorrect pasword!" << std::endl;
        }
    }

    void call_apartment(uint16_t &flat)
    {
        if  (flat_is_real(flat)) {
            std::cout << "Calling to flat " << flat << "..." << std::endl;
        } else {
            std::cout << "No apartment with this number!" << std::endl;
        }
    }

    uint16_t extract_flat_number(const std::vector<char>& buf)
    {
        return static_cast<uint16_t>((buf.at(0) - kIndent) * 100 + (buf.at(1) - kIndent) * 10 + (buf.at(2) - kIndent));
    }

    bool flat_is_real(uint16_t &flat)
    {
        return ((flat >= kFirstFlat) && (flat <= kLast_Flat));
    }

    bool is_number(const char symbol)
    {
        if ((symbol >= kChar0) && (symbol <= kChar9))
        {
            return true;
        }
        return false;
    }

    static constexpr uint16_t kFirstFlat{65};
    static constexpr uint16_t kLast_Flat{120};
    static constexpr uint16_t kIndent{48};
    static constexpr uint8_t kPasswordLength{4};
    static constexpr char kBackspace{127};
    static constexpr char kChar0{0x30};
    static constexpr char kChar9{0x39};



    enum  State : uint8_t {
        Symbol1 = 1,
        Symbol2 = 2,
        Symbol3 = 3,
        Symbol4 = 4,
        Symbol5 = 5,
        Symbol6 = 6,
        Symbol7 = 7,
        Symbol8 = 8,
        Symbol9 = 9
    } state;

    std::vector<char> buffer;
    char password[kPasswordLength]{'5', '6', '7', '8'};
};

#endif // INTERCOM_HPP
