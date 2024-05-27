#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct TR
{
    std::string SC;//SourceCurrency
    std::string DC;//DestibationCurrency
    float Lots;
    double Price;
};

class Processor
{
public:
    void Process(std::istream& stream);
    
private:
    static const float LotSize;
    std::vector<std::string> SplitString(const std::string& str, char delimiter);
    bool CheckIntegerValidity(const std::string& str, int& value);
    bool CheckDoubleValidity(const std::string& str, double& value);
    bool ProcessLine(const std::string& line, TR& trade, int lineNumber);
    std::string GenerateXML(const std::vector<TR>& trades);
};
const float Processor::LotSize = 100000;

// int main()
// {
//     TradeProcessor processor;
//     std::ifstream file("trades.txt");
//     if (file.is_open())
//     {
//         processor.ProcessTrades(file);
//         file.close();
//     }
//     else
//     {
//         std::cout << "Failed to open file" << std::endl;
//     }
//     return 0;
// }

