/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 INESC TEC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Pedro Silva  <pmms@inesctec.pt>
 * Maintainer: Viveris Technologies\n"
 *
 * Code from https://gitlab.inesctec.pt/pmms/ns3-satellite
 *
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <string>
#include <vector>
#include <exception>


const double ArcSec2Rad = 4.8481368110954e-06;


/**
 * @brief The EopParameters struct for Earth Orientation Parameters.
 *
 * Stores polar motion values (xp, yp), DUT1 (UT1-UTC), and length-of-day (LOD).
 */
struct EopParameters
{
    double xp, yp, dut1, lod;
};


/**
 * @brief Open a file in read mode
 */
std::ifstream
OpenFile(const std::string& filename)
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(filename);
    } catch (...) {
        std::cerr << "Unable to open file '" << filename << "'\n";
        std::rethrow_exception(std::current_exception());
    }
    return file;
}


/**
 * @brief Safe readline for reading files created in a platform that uses different
 *        newline characters (Mac OS: '\r', Linux: '\n', Windows: '\r\n')
 */
std::istream&
ReadLine(std::istream& in, std::string& str)
{
    bool carriage_return = false;

    str.clear();
    while (true)
    {
        char c = static_cast<char>(in.peek());

        switch (c)
        {
        case '\n':
            in.ignore(1);
            return in;
        case '\r':
            in.ignore(1);
            carriage_return = true;
            break;
        case EOF:
            return in;
        default:
            // if this is a Mac OS file
            if (carriage_return)
                return in;

            str += in.get();
        }
    }

    return in;
}


std::vector<uint32_t>
ReadLeapSeconds(const std::string& filename)
{
    std::vector<uint32_t> v;
    std::string line;
    uint32_t linenum = 0;

    std::ifstream f = OpenFile(filename);
    while (ReadLine(f, line))
    {
        // ignore the first 14 lines
        if (++linenum <= 14)
        {
            continue;
        }

        if (line.empty())
        {
            break;
        }

        std::istringstream iss(line);
        iss.ignore(line.size(), ' '); // ignore year
        iss.ignore(line.size(), ' '); // ignore month
        iss.ignore(line.size(), '='); // ignore day
        iss.ignore(line.size(), ' '); // ignore =JD

        uint32_t jd;
        iss >> jd;

        // time relative to Unix epoch for when leap second was introduced
        v.push_back(jd - 2440587);
    }

    f.close();
    return v;
}


std::vector<EopParameters>
ReadEopParameters(const std::string& filename)
{
    std::vector<EopParameters> v;
    std::string line;

    std::ifstream f = OpenFile(filename);
    while (ReadLine(f, line))
    {
        // if there are no more valid parameters to be read
        if (line.empty() || line.size() < 185 || line[16] == ' ')
        {
            break;
        }

        EopParameters eop;
        double dev_null;

        std::istringstream iss;
        iss.str(line.substr(18)); // move to polar motion
        iss >> eop.xp;            // read polar motion x
        iss >> dev_null;          // ignore xp error
        iss >> eop.yp;            // read polar motion y

        iss.str(line.substr(58)); // move to DUT1
        iss >> eop.dut1;          // read DUT1
        iss >> dev_null;          // ignore DUT1 error

        iss.str(line.substr(79, 7)); // just LOD field
        if (iss.str() == "       ") // LOD not filled
        {
            eop.lod = 0;
        }
        else // LOD filled
        {
            iss >> eop.lod;
        }

        eop.xp *= ArcSec2Rad; // convert to radians
        eop.yp *= ArcSec2Rad; // convert to radians

        v.push_back(eop);
    }

    f.close();
    return v;
}


void
PrintCopyright(std::ofstream& f)
{
    f << "/* -*-  Mode: C++; c-file-style: \"gnu\"; indent-tabs-mode:nil; -*- "
         "*/\n"
         "/*\n"
         " * Copyright (c) 2016 INESC TEC\n"
         " *\n"
         " * This program is free software; you can redistribute it and/or modify\n"
         " * it under the terms of the GNU General Public License version 2 as\n"
         " * published by the Free Software Foundation;\n"
         " *\n"
         " * This program is distributed in the hope that it will be useful,\n"
         " * but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
         " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
         " * GNU General Public License for more details.\n"
         " *\n"
         " * You should have received a copy of the GNU General Public License\n"
         " * along with this program; if not, write to the Free Software\n"
         " * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA\n"
         " *\n"
         " * Author: Pedro Silva  <pmms@inesctec.pt>\n"
         " * Maintainer: Viveris Technologies\n"
         " *\n"
         " */\n\n";
}


void
PrintAutoGenInfo(std::ofstream& f)
{
    f << "/*\n"
      << " * IMPORTANT: This file was generated automatically. Do not change it!\n"
      << " *            Instead, update files 'tai-utc.dat', and 'finals.data' on\n"
      << " *            data/sgp4/ folder using the latest files\n"
      << " *            provided at:\n"
      << " * http://www.usno.navy.mil/USNO/earth-orientation/eo-info/general/bullc\n"
      << " * and\n"
      << " * http://www.usno.navy.mil/USNO/earth-orientation/eo-products/weekly\n"
      << " * or\n"
      << " * http://www.iers.org/IERS/EN/DataProducts/EarthOrientationData/eop.html\n */"
      << "\n\n";
}


void
PrintStartNamespace(std::ofstream& f)
{
    f << "namespace ns3\n{\n\n";
}


void
PrintEndNamespace(std::ofstream& f)
{
    f << "} // namespace ns3\n";
}


void
PrintLeapSeconds(std::ofstream& f, const std::vector<uint32_t>& leap)
{
    f << "const uint32_t IersData::BaseLeapSeconds = 10;" << std::endl
      << "const uint32_t IersData::CurLeapSeconds = " << leap.size() + 10 << ";" << std::endl;

    if (!leap.size())
    {
        return;
    }

    f << "const uint32_t IersData::__leap_secs[] = {\n  " << leap.at(0);
    for (uint32_t i = 1; i < leap.size(); ++i)
    {
        f << ",";

        if ((i & 0x07) == 0)
        {
            f << "\n  ";
        }
        else
        {
            f << " ";
        }

        f << leap.at(i);
    }

    f << "\n};\n"
      << "// Modified Julian Date for when leap seconds were added in UTC\n"
      << "const std::vector<uint32_t> IersData::LeapSeconds(\n"
      << "  __leap_secs, __leap_secs + " << leap.size() << "\n"
      << ");\n"
      << std::endl;
}


void
PrintEopParams(std::ofstream& f, const EopParameters& eop)
{
    f << "{" << eop.xp << "," << eop.yp << "," << eop.dut1 << "," << eop.lod << "}";
}


void
PrintEopParameters(std::ofstream& f, const std::vector<EopParameters>& eop)
{
    if (!eop.size())
    {
        return;
    }

    f << "const IersData::EopParameters IersData::__eop_params[] = {\n  ";
    PrintEopParams(f, eop.at(0));
    for (uint32_t i = 1; i < eop.size(); ++i)
    {
        f << ",";

        if ((i & 0x01) == 0)
        {
            f << "\n  ";
        }
        else
        {
            f << " ";
        }

        PrintEopParams(f, eop.at(i));
    }

    f << "\n};\n"
      << "// Daily EOP parameters since 01 Jan 1992 UTC\n"
      << "const std::vector<IersData::EopParameters> IersData::EopValues(\n"
      << "  __eop_params, __eop_params + " << eop.size() << "\n"
      << ");\n"
      << std::endl;
}


void
GenerateHeaderFile(const std::string& filename)
{
    std::ofstream out;
    out.open(filename);
    if (!out)
    {
        std::cerr << "Unable to create file '" << filename << "'" << std::endl;
        return;
    }

    PrintCopyright(out);
    PrintAutoGenInfo(out);
    out << "#ifndef IERS_DATA_H\n"
        << "#define IERS_DATA_H\n\n"
        << "#include <stdint.h>\n"
        << "#include <vector>\n\n";

    PrintStartNamespace(out);
    out << "class IersData\n"
        << "{\n"
        << "  public:\n"
        << "    struct EopParameters\n"
        << "    {\n"
        << "      // radians, radians, seconds, milliseconds\n"
        << "      double xp, yp, dut1, lod;\n"
        << "    };\n\n"
        << "    static const std::vector<EopParameters> EopValues;\n"
        << "    static const uint32_t BaseLeapSeconds;\n"
        << "    static const uint32_t CurLeapSeconds;\n"
        << "    static const std::vector<uint32_t> LeapSeconds;\n\n"
        << "  protected:\n"
        << "    static const EopParameters __eop_params[];\n"
        << "    static const uint32_t __leap_secs[];\n"
        << "};\n\n";

    PrintEndNamespace(out);
    out << "\n#endif // IERS_DATA_H" << std::endl;
    out.close();
}


void
GenerateImplementationFile(const std::string& filename,
                           const std::vector<EopParameters>& eop,
                           const std::vector<uint32_t>& leap)
{
    std::ofstream out;
    out.open(filename);
    if (!out)
    {
        std::cerr << "Unable to create file '" << filename << "'" << std::endl;
        return;
    }

    PrintCopyright(out);
    PrintAutoGenInfo(out);

    out << "#include \"iers-data.h\"\n\n";
    PrintStartNamespace(out);

    PrintEopParameters(out, eop);
    PrintLeapSeconds(out, leap);

    PrintEndNamespace(out);

    out.close();
}


int
main(int argc, char* argv[])
{
    if (argc < 3 || argc > 4)
    {
        std::cerr << "Usage: " << argv[0]
                  << " leap_filepath eop_filepath [output_folder]\n";
        return 1;
    }

    std::string leap_path(argv[1]);
    std::string eop_path(argv[2]);
    std::string output_path;

    if (argc == 4)
    {
        output_path = std::string(argv[3]);
    }

    if (!output_path.empty() && output_path.back() != '/')
    {
      output_path = output_path + "/";
    }

    std::vector<uint32_t> leap = ReadLeapSeconds(leap_path);
    std::vector<EopParameters> eop = ReadEopParameters(eop_path);

    GenerateHeaderFile(output_path + "iers-data.h");
    GenerateImplementationFile(output_path + "iers-data.cc", eop, leap);

    return 0;
}
