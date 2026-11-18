#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <sstream>
#include <cstdint>
#include <algorithm>
#include <iomanip>

struct Student_name {
    int num;
    std::string surname;
    std::string name;
    std::string patr;
};

struct Student_marks {
    int group, num;
    std::string subj[3];
    int marks[3];
};

struct Student {
    std::string surname;
    int group, num;
    std::string subj[3];
    int marks[3];
};

struct Long_st {
    std::string surname;
    int group, num;
    std::string subj[3];
    int marks[3];
    double avg;
};

struct Short_st {
    std::string surname;
    int group, num;
};

//Á‡‰‡ÌËÂ a
void check_file(std::ifstream& file) {
    if (!file.is_open()) throw "file error open";
    if (file.peek() == EOF) throw "file error empty";
}

void check_open_file(std::ofstream& file) {
    if (!file.is_open()) throw "file error open";
}

std::string read_without_puncts(std::ifstream& file, const std::string& delim) {
    std::string line, result;
    while (std::getline(file, line)) {
        for (char c : line) {
            if (delim.find(c) == std::string::npos) {
                result += c;
            }
            else result += " ";
        }
        result += '\n';
    }
    return result;
}

int return_size(std::ifstream& file) {
    int total{};
    std::string line;

    while (std::getline(file, line)) ++total;
    file.clear();
    file.seekg(0);

    return total;
}

Student_name* fill_mass_name(std::ifstream& file, int size) {
    std::string delim = ",.;:?";
    std::string text = read_without_puncts(file, delim);

    Student_name* mass = new Student_name[size]{};
    std::istringstream iss(text);

    int book_num{};
    std::string w1, w2, w3;

    int temp = 0;
    while ((temp < size) && (iss >> book_num >> w1 >> w2 >> w3)) {
        mass[temp].num = book_num;
        mass[temp].surname = w1;
        mass[temp].name = w2;
        mass[temp].patr = w3;
        ++temp;
    }

    return mass;
}

Student_marks* fill_mass_marks(std::ifstream& file, int size) {
    std::string delim = ",.;:?";
    std::string text = read_without_puncts(file, delim);

    Student_marks* mass = new Student_marks[size]{};
    std::istringstream iss(text);

    int group_num{}, book_num{};
    int mark0{}, mark1{}, mark2{};
    std::string w0, w1, w2;

    int temp = 0;
    while ((temp < size) && (iss >> group_num >> book_num >>
        w0 >> mark0 >> w1 >> mark1 >> w2 >> mark2)) {
        mass[temp].group = group_num;
        mass[temp].num = book_num;

        mass[temp].marks[0] = mark0;
        mass[temp].marks[1] = mark1;
        mass[temp].marks[2] = mark2;

        mass[temp].subj[0] = w0;
        mass[temp].subj[1] = w1;
        mass[temp].subj[2] = w2;
        ++temp;
    }

    return mass;
}

void write_str(std::ofstream& file, const std::string& s) {
    uint32_t len = static_cast<uint32_t>(s.size());
    file.write(reinterpret_cast<const char*>(&len), sizeof(len));
    file.write(s.data(), len);
}

bool read_str(std::ifstream& file, std::string& s) {
    uint32_t len = 0;
    file.read(reinterpret_cast<char*>(&len), sizeof(len));
    if (!file) return {};
    s.resize(len);
    return static_cast<bool>(file.read(s.data(), len));
}

void write_student_name(std::ofstream& out, const Student_name& s) {
    out.write(reinterpret_cast<const char*>(&s.num), sizeof(s.num));
    write_str(out, s.name);
    write_str(out, s.surname);
    write_str(out, s.patr);
}

bool read_student_name(std::ifstream& in, Student_name& s) {
    if (!in.read(reinterpret_cast<char*>(&s.num), sizeof(s.num))) return false;
    if (!read_str(in, s.name)) return false;
    if (!read_str(in, s.surname)) return false;
    if (!read_str(in, s.patr)) return false;
    return true;
}

void write_student_marks(std::ofstream& out, const Student_marks& s) {
    out.write(reinterpret_cast<const char*>(&s.group), sizeof(s.group));
    out.write(reinterpret_cast<const char*>(&s.num), sizeof(s.num));

    for (int i{}; i < 3; ++i) {
        write_str(out, s.subj[i]);
    }

    out.write(reinterpret_cast<const char*>(&s.marks), sizeof(s.marks));
}

bool read_student_marks(std::ifstream& in, Student_marks& s) {
    if (!in.read(reinterpret_cast<char*>(&s.group), sizeof(s.group))) return false;
    if (!in.read(reinterpret_cast<char*>(&s.num), sizeof(s.num))) return false;

    for (int i{}; i < 3; ++i) {
        if (!read_str(in, s.subj[i])) return false;
    }

    if (!in.read(reinterpret_cast<char*>(&s.marks), sizeof(s.marks))) return false;
    return true;
}

void fill_bin_file_name(std::ofstream& out, Student_name* mass, int size) {
    for (int i{}; i < size; ++i) {
        write_student_name(out, mass[i]);
    }
}

void fill_bin_file_marks(std::ofstream& out, Student_marks* mass, int size) {
    for (int i{}; i < size; ++i) {
        write_student_marks(out, mass[i]);
    }
}

Student_name* bin_to_str_name(std::ifstream& in, int size) {
    Student_name* s = new Student_name[size]{};
    int i{};
    while ((i < size) && (read_student_name(in, s[i]))) {
        ++i;
    }
    return s;
}

Student_marks* bin_to_str_marks(std::ifstream& in, int size) {
    Student_marks* s = new Student_marks[size]{};
    int i{};
    while ((i < size) && (read_student_marks(in, s[i]))) {
        ++i;
    }
    return s;
}

void print_struct_name(Student_name* s, int size) {
    for (int i{}; i < size; ++i) {
        std::cout << s[i].num << " " << s[i].name << " " << s[i].surname
            << " " << s[i].patr << " " << '\n';
    }
}

void print_struct_marks(Student_marks* s, int size) {
    for (int i{}; i < size; ++i) {
        std::cout << s[i].group << " " << s[i].num << " ";
        std::cout << s[i].subj[0] << " " << s[i].marks[0] << " ";
        std::cout << s[i].subj[1] << " " << s[i].marks[1] << " ";
        std::cout << s[i].subj[2] << " " << s[i].marks[2] << " " << '\n';
    }
}

//Á‡‰‡ÌËÂ ·
Student* fill_student(Student_name* s1, Student_marks* s2, int size) {
    Student* s = new Student[size]{};
    for (int i{}; i < size; ++i) {
        for (int j{}; j < size; ++j) {
            if (s1[i].num == s2[j].num) {
                s[i].surname = s1[i].surname;
                s[i].group = s2[j].group;
                s[i].num = s1[i].num;
                for (int j1{}; j1 < 3; ++j1) {
                    s[i].subj[j1] = s2[j].subj[j1];
                }
                for (int j2{}; j2 < 3; ++j2) {
                    s[i].marks[j2] = s2[j].marks[j2];
                }
            }
        }
    }
    return s;
}

void write_student(std::ofstream& out, const Student& s) {
    write_str(out, s.surname);

    out.write(reinterpret_cast<const char*>(&s.group), sizeof(s.group));
    out.write(reinterpret_cast<const char*>(&s.num), sizeof(s.num));

    for (int i{}; i < 3; ++i) {
        write_str(out, s.subj[i]);
    }

    out.write(reinterpret_cast<const char*>(&s.marks), sizeof(s.marks));
}

void fill_bin_file(std::ofstream& out, Student* mass, int size) {
    for (int i{}; i < size; ++i) {
        write_student(out, mass[i]);
    }
}

bool read_student(std::ifstream& in, Student& s) {
    if (!read_str(in, s.surname)) return false;

    if (!in.read(reinterpret_cast<char*>(&s.group), sizeof(s.group))) return false;
    if (!in.read(reinterpret_cast<char*>(&s.num), sizeof(s.num))) return false;

    for (int i{}; i < 3; ++i) {
        if (!read_str(in, s.subj[i])) return false;
    }

    if (!in.read(reinterpret_cast<char*>(&s.marks), sizeof(s.marks))) return false;
    return true;
}

Student* bin_to_str(std::ifstream& in, int size) {
    Student* s = new Student[size]{};
    int i{};
    while ((i < size) && (read_student(in, s[i]))) {
        ++i;
    }
    return s;
}

void print_struct(Student* s, int size) {
    for (int i{}; i < size; ++i) {
        std::cout << s[i].surname << " ";
        std::cout << s[i].group << " " << s[i].num << " ";
        std::cout << s[i].subj[0] << " " << s[i].marks[0] << " ";
        std::cout << s[i].subj[1] << " " << s[i].marks[1] << " ";
        std::cout << s[i].subj[2] << " " << s[i].marks[2] << " " << '\n';
    }
    std::cout << '\n';
}

//Á‡‰‡ÌËÂ ‚
double avg(int* mass, int len) {
    double total{};
    for (int i{}; i < len; ++i) total += mass[i];
    return total / len;
}

void write_long_st(std::ofstream& out, const Long_st& s) {
    write_str(out, s.surname);
    out.write(reinterpret_cast<const char*>(&s.group), sizeof(s.group));
    out.write(reinterpret_cast<const char*>(&s.num), sizeof(s.num));

    int len = sizeof(s.marks) / sizeof(s.marks[0]);
    for (int i = 0; i < len; ++i) {
        write_str(out, s.subj[i]);
    }

    out.write(reinterpret_cast<const char*>(&s.marks), sizeof(s.marks));
    out.write(reinterpret_cast<const char*>(&s.avg), sizeof(s.avg));
}

void fill_bin_file_long(std::ofstream& out, Long_st* mass, int size) {
    for (int i = 0; i < size; ++i) {
        write_long_st(out, mass[i]);
    }
}

Long_st* st_to_long_st(Student* s1, int size) {
    Long_st* s = new Long_st[size]{};
    int len = sizeof(s1[0].marks) / sizeof(s1[0].marks[0]);
    for (int i{}; i < size; ++i) {
        double avg_marks = avg(s1[i].marks, len);
        s[i].surname = s1[i].surname;
        s[i].group = s1[i].group;
        s[i].num = s1[i].num;
        for (int j{}; j < 3; ++j) {
            s[i].subj[j] = s1[i].subj[j];
            s[i].marks[j] = s1[i].marks[j];
        }
        s[i].avg = avg_marks;
    }
    return s;
}

bool read_student_long(std::ifstream& in, Long_st& s) {
    if (!read_str(in, s.surname)) return false;

    if (!in.read(reinterpret_cast<char*>(&s.group), sizeof(s.group))) return false;
    if (!in.read(reinterpret_cast<char*>(&s.num), sizeof(s.num))) return false;

    for (int i{}; i < 3; ++i) {
        if (!read_str(in, s.subj[i])) return false;
    }

    if (!in.read(reinterpret_cast<char*>(&s.marks), sizeof(s.marks))) return false;
    if (!in.read(reinterpret_cast<char*>(&s.avg), sizeof(s.avg))) return false;
    return true;
}

Long_st* bin_to_str_long(std::ifstream& in, int size) {
    Long_st* s = new Long_st[size]{};
    int i{};
    while ((i < size) && (read_student_long(in, s[i]))) {
        ++i;
    }
    return s;
}

//Á‡‰‡ÌËÂ „
int return_bad_size(Long_st* s, int size) {
    int size_bad{};

    for (int i{}; i < size; ++i) {
        for (int j{}; j < 3; ++j) {
            if (s[i].marks[j] < 4) {
                ++size_bad;
                break;
            }
        }
    }
    return size_bad;
}

Short_st* bad_students(Long_st* s, int size) {
    int size_bad = return_bad_size(s, size);
    Short_st* st_bad = new Short_st[size_bad]{};

    for (int i1{}, i2{}; (i1 < size) && (i2 < size_bad); ++i1) {
        for (int j{}; j < 3; ++j) {
            if (s[i1].marks[j] < 4) {
                st_bad[i2].surname = s[i1].surname;
                st_bad[i2].group = s[i1].group;
                st_bad[i2].num = s[i1].num;
                ++i2;
                break;
            }
        }
    }
    return st_bad;
}

void write_bad_student(std::ofstream& out, const Short_st& s) {
    write_str(out, s.surname);

    out.write(reinterpret_cast<const char*>(&s.group), sizeof(s.group));
    out.write(reinterpret_cast<const char*>(&s.num), sizeof(s.num));
}

void fill_bin_file_bad(std::ofstream& out, Short_st* mass, int size) {
    for (int i{}; i < size; ++i) {
        write_bad_student(out, mass[i]);
    }
}

bool read_bad_student(std::ifstream& in, Short_st& s) {
    if (!read_str(in, s.surname)) return false;

    if (!in.read(reinterpret_cast<char*>(&s.group), sizeof(s.group))) return false;
    if (!in.read(reinterpret_cast<char*>(&s.num), sizeof(s.num))) return false;

    return true;
}

Short_st* bin_to_str_bad(std::ifstream& in, int size) {
    Short_st* s = new Short_st[size]{};
    int i{};
    while ((i < size) && (read_bad_student(in, s[i]))) {
        ++i;
    }
    return s;
}

//ÙÛÌÍˆËˇ ÒÓÚËÓ‚ÍË ÔÓ „ÛÔÔ‡Ï Ë ÔÓ Ù‡ÏËÎËˇÏ
void struct_sort(Long_st* s, int size) {
    for (int i{}; i < size - 1; ++i) {
        bool flag = false;
        for (int j{}; j < size - i - 1; ++j) {
            if (s[j].group > s[j + 1].group) {
                std::swap(s[j], s[j + 1]);
                flag = true;
            }
            else if (s[j].group == s[j + 1].group) {
                if (s[j].surname > s[j + 1].surname) {
                    std::swap(s[j], s[j + 1]);
                    flag = true;
                }
            }
        }
        if (!flag) break;
    }
}

void print_short_st(Short_st* s, int size) {
    if (size == 0) std::cout << "—œ»—Œ  œ”—“\n";
    for (int i{}; i < size; ++i) {
        std::cout << s[i].surname << " ";
        std::cout << s[i].group << " " << s[i].num << '\n';
    }
    std::cout << '\n';
}

void print_long_st(Long_st* s, int size) {
    for (int i{}; i < size; ++i) {
        std::cout << s[i].surname << " ";
        std::cout << s[i].group << " " << s[i].num << " ";
        std::cout << s[i].subj[0] << " " << s[i].marks[0] << " ";
        std::cout << s[i].subj[1] << " " << s[i].marks[1] << " ";
        std::cout << s[i].subj[2] << " " << s[i].marks[2] << " ";
        std::cout << s[i].avg << '\n';
    }
    std::cout << '\n';
}

//Á‡‰‡ÌËÂ Ê
Long_st* vedomost_group_alph(Long_st* s, int size, int num) {
    Long_st* s_gr = new Long_st[size]{};
    bool flag = false;
    for (int i{}, j{}; i < size; ++i) {
        if (s[i].group == num) {
            flag = true;
            s_gr[j++] = s[i];
        }
        else { if (flag) break; }
    }
    return s_gr;
}

int size_group(Long_st* s_gr, int size) {
    int counter{};
    while ((counter < size) && (s_gr[counter].group != 0)) {
        ++counter;
    }
    return counter;
}

//Á‡‰‡ÌËÂ Ë
int return_best_size(Long_st* s, int size) {
    int size_best{};

    for (int i{}; i < size; ++i) {
        if (s[i].avg >= 9) {
            ++size_best;
        }
    }
    return size_best;
}

Short_st* best_students(Long_st* s, int size) {
    int size_best = return_best_size(s, size);
    Short_st* st_best = new Short_st[size_best]{};

    for (int i1{}, i2{}; (i1 < size) && (i2 < size_best); ++i1) {
        if (s[i1].avg >= 9) {
            st_best[i2].surname = s[i1].surname;
            st_best[i2].group = s[i1].group;
            st_best[i2].num = s[i1].num;
            ++i2;
        }
    }
    return st_best;
}

//Á‡‰‡ÌËÂ Á
void struct_sort_avg(Long_st* s, int size) {
    for (int i{}; i < size - 1; ++i) {
        bool flag = false;
        for (int j{}; j < size - i - 1; ++j) {
            if (s[j].avg < s[j + 1].avg) {
                std::swap(s[j], s[j + 1]);
                flag = true;
            }
        }
        if (!flag) return;
    }
}

Long_st* vedomost_group_avg(Long_st* s, int size, int num) {
    Long_st* s_gr = vedomost_group_alph(s, size, num);
    int size_gr = size_group(s_gr, size);
    struct_sort_avg(s_gr, size_gr);
    return s_gr;
}

//Á‡‰‡ÌËÂ 4
void write_long_st_txt(std::ofstream& out, const Long_st& s) {
    out << "‘¿Ã»À»ﬂ:" << std::setw(10) << s.surname << " √–”œœ¿: " << s.group
        << " ÕŒÃ≈–: " << s.num << " ";
    int len = sizeof(s.marks) / sizeof(s.marks[0]);
    for (int i{}; i < len; ++i) {
        out << s.subj[i] << ": " << std::setw(2) << s.marks[i] << " ";
    }
    out << "—–: " << s.avg << '\n';
}

void fill_txt_long(std::ofstream& out, Long_st* s, int size) {
    for (int i{}; i < size; ++i) {
        write_long_st_txt(out, s[i]);
    }
}

void write_short_st_txt(std::ofstream& out, const Short_st& s) {
    out << "‘¿Ã»À»ﬂ:" << std::setw(10) << s.surname << " √–”œœ¿: " << s.group
        << " ÕŒÃ≈–: " << s.num << '\n';
}

void fill_txt_short(std::ofstream& out, Short_st* s, int size) {
    for (int i{}; i < size; ++i) {
        write_short_st_txt(out, s[i]);
    }
}

void print_txt(std::ifstream& in) {
    std::string line;
    while (std::getline(in, line)) {
        std::cout << line << '\n';
    }
}

bool cmp_long_avg(const Long_st& a, const Long_st& b) {
    return a.avg > b.avg;
}

Long_st* vedomost_group_avg_standart(Long_st* s, int size, int num) {
    Long_st* s_gr = vedomost_group_alph(s, size, num);
    int size_gr = size_group(s_gr, size);
    std::sort(s_gr, s_gr + size_gr, cmp_long_avg);
    return s_gr;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::ifstream fin1("students_name.txt");
    try { check_file(fin1); }
    catch (const char* msg) { std::cout << msg; return 1; }

    std::ifstream fin2("students_marks.txt");
    try { check_file(fin2); }
    catch (const char* msg) { std::cout << msg; return 1; }

    int size = return_size(fin1);

    std::ofstream fout1("students_name.bin", std::ios::binary);
    try { check_open_file(fout1); }
    catch (const char* msg) { std::cout << msg; return 1; }

    std::ofstream fout2("students_marks.bin", std::ios::binary);
    try { check_open_file(fout2); }
    catch (const char* msg) { std::cout << msg; return 1; }

    Student_name* names = fill_mass_name(fin1, size);
    Student_marks* marks = fill_mass_marks(fin2, size);

    fill_bin_file_name(fout1, names, size);
    fill_bin_file_marks(fout2, marks, size);

    fout1.close(); fout2.close();

    delete[] names; delete[] marks;

    std::ifstream fin_bin1("students_name.bin", std::ios::binary);
    try { check_file(fin_bin1); }
    catch (const char* msg) { std::cout << msg; return 1; }

    std::ifstream fin_bin2("students_marks.bin", std::ios::binary);
    try { check_file(fin_bin2); }
    catch (const char* msg) { std::cout << msg; return 1; }

    Student_name* stud_name = bin_to_str_name(fin_bin1, size);
    Student_marks* stud_marks = bin_to_str_marks(fin_bin2, size);

    std::cout << "—Œƒ≈–∆»ÃŒ≈ œ≈–¬Œ√Œ ¡»Õ¿–ÕŒ√Œ ‘¿…À¿:\n";
    print_struct_name(stud_name, size);
    std::cout << "\n—Œƒ≈–∆»ÃŒ≈ ¬“Œ–Œ√Œ ¡»Õ¿–ÕŒ√Œ ‘¿…À¿:\n";
    print_struct_marks(stud_marks, size);

    fin_bin1.close(); fin_bin2.close();

    //Á‡‰‡ÌËÂ ·
    std::ofstream fout("stud.bin", std::ios::binary);
    try { check_open_file(fout); }
    catch (const char* msg) { std::cout << msg; return 1; }

    Student* student = fill_student(stud_name, stud_marks, size);
    fill_bin_file(fout, student, size);
    fout.close();

    std::ifstream fin("stud.bin", std::ios::binary);
    try { check_file(fin); }
    catch (const char* msg) { std::cout << msg; return 1; }

    Student* st_gap0 = bin_to_str(fin, size);
    std::cout << "\n—Œƒ≈–∆»ÃŒ≈ ¡»Õ¿–ÕŒ√Œ ‘¿…À¿ «¿ƒ¿Õ»≈ ¡:\n";
    print_struct(st_gap0, size);
    delete[] st_gap0;
    fin.close();

    //Á‡‰‡ÌËÂ ‚
    std::ofstream fout_avg("stud_with_avg.bin", std::ios::binary);
    try { check_open_file(fout_avg); }
    catch (const char* msg) { std::cout << msg; return 1; }

    Long_st* long_st = st_to_long_st(student, size);

    fill_bin_file_long(fout_avg, long_st, size);
    fout_avg.close();

    std::ifstream fin_avg("stud_with_avg.bin", std::ios::binary);
    try { check_file(fin_avg); }
    catch (const char* msg) { std::cout << msg; return 1; }

    Long_st* long_st_gap0 = bin_to_str_long(fin_avg, size);
    std::cout << "—Œƒ≈–∆»ÃŒ≈ ¡»Õ¿–ÕŒ√Œ ‘¿…À¿ «¿ƒ¿Õ»≈ ¬:\n";
    print_long_st(long_st_gap0, size);
    delete[] long_st_gap0;
    fin_avg.close();

    //«¿ƒ¿Õ»ﬂ √, ƒ, ≈
    std::ofstream bad_before_sort_out("bad_before_sort.bin", std::ios::binary);
    try { check_open_file(bad_before_sort_out); }
    catch (const char* msg) { std::cout << msg; return 1; }

    int size_bad = return_bad_size(long_st, size);
    Short_st* st_bad1 = bad_students(long_st, size);

    fill_bin_file_bad(bad_before_sort_out, st_bad1, size_bad);
    bad_before_sort_out.close();
    delete[] st_bad1;

    std::ifstream bad_before_sort_in("bad_before_sort.bin", std::ios::binary);
    if (size_bad > 0) {
        try { check_file(bad_before_sort_in); }
        catch (const char* msg) { std::cout << msg; return 1; }

        Short_st* st_bad1_gap = bin_to_str_bad(bad_before_sort_in, size_bad);
        std::cout << "—Œƒ≈–∆»ÃŒ≈ ¡»Õ¿–ÕŒ√Œ ‘¿…À¿ Õ≈”—œ≈¬¿Ÿ»’ —“”ƒ≈Õ“Œ¬ ƒŒ —Œ–“»–Œ¬ »:\n";
        print_short_st(st_bad1_gap, size_bad);
        delete[] st_bad1_gap;
    }
    else { std::cout << "—œ»—Œ  Õ≈”—œ≈¬¿ﬁŸ»’ œ”—“\n"; }
    bad_before_sort_in.close();

    struct_sort(long_st, size);

    std::ofstream bad_after_sort_out("bad_after_sort.bin", std::ios::binary);
    try { check_open_file(bad_after_sort_out); }
    catch (const char* msg) { std::cout << msg; return 1; }

    Short_st* st_bad_sort = bad_students(long_st, size);

    fill_bin_file_bad(bad_after_sort_out, st_bad_sort, size_bad);
    bad_after_sort_out.close();

    std::ifstream bad_after_sort_in("bad_after_sort.bin", std::ios::binary);
    if (size_bad > 0) {
        try { check_file(bad_after_sort_in); }
        catch (const char* msg) { std::cout << msg; return 1; }

        Short_st* st_bad2_gap = bin_to_str_bad(bad_after_sort_in, size_bad);
        std::cout << "—Œƒ≈–∆»ÃŒ≈ ¡»Õ¿–ÕŒ√Œ ‘¿…À¿ Õ≈”—œ≈¬¿Ÿ»’ —“”ƒ≈Õ“Œ¬ œŒ—À≈ —Œ–“»–Œ¬ »:\n";
        print_short_st(st_bad2_gap, size_bad);
        delete[] st_bad2_gap;
    }
    else { std::cout << "—œ»—Œ  Õ≈”—œ≈¬¿ﬁŸ»’ œ”—“\n"; }
    bad_after_sort_in.close();

    //Á‡‰‡ÌËÂ Ê
    int num1 = 4;
    std::cout << "ÕŒÃ≈– √–”œœ€ ƒÀﬂ ‘Œ–Ã»–Œ¬¿Õ»ﬂ ¬≈ƒŒÃŒ—“» Œ÷≈ÕŒ  (”œŒ–ﬂƒŒ◊≈Õ€ œŒ ¿À‘¿¬»“”): " << num1 << '\n';
    //std::cin >> num1;

    Long_st* s_gr1 = vedomost_group_alph(long_st, size, num1);
    int size_gr1 = size_group(s_gr1, size);

    std::ofstream fout_g("vedomost_group.bin", std::ios::binary);
    try { check_open_file(fout_g); }
    catch (const char* msg) { std::cout << msg; return 1; }

    fill_bin_file_long(fout_g, s_gr1, size_gr1);
    fout_g.close();

    std::ifstream fin_g("vedomost_group.bin", std::ios::binary);
    if (size_gr1 > 0) {
        try { check_file(fin_g); }
        catch (const char* msg) { std::cout << msg; return 1; }

        Long_st* s_gr1_from_file = bin_to_str_long(fin_g, size_gr1);
        fin_g.close();

        std::cout << "¬≈ƒŒÃŒ—“‹ √–”œœ€ " << num1 << " (œŒ ¿À‘¿¬»“”):\n";
        print_long_st(s_gr1_from_file, size_gr1);

        delete[] s_gr1_from_file;
    }
    else { std::cout << "“¿ Œ… √–”œœ€ Õ≈“\n"; }
    fin_g.close();

    //Á‡‰‡ÌËÂ Á
    int num2 = 5;
    std::cout << "ÕŒÃ≈– √–”œœ€ ƒÀﬂ ‘Œ–Ã»–Œ¬¿Õ»ﬂ ¬≈ƒŒÃŒ—“» Œ÷≈ÕŒ  (”œŒ–ﬂƒŒ◊≈Õ€ œŒ —–.¡¿ÀÀ”): " << num2 << '\n';
    //std::cin >> num2;

    Long_st* s_gr2 = vedomost_group_avg(long_st, size, num2);
    int size_gr2 = size_group(s_gr2, size);

    std::ofstream fout_h("vedomost_group_avg.bin", std::ios::binary);
    try { check_open_file(fout_h); }
    catch (const char* msg) { std::cout << msg; return 1; }

    fill_bin_file_long(fout_h, s_gr2, size_gr2);
    fout_h.close();

    std::ifstream fin_h("vedomost_group_avg.bin", std::ios::binary);
    if (size_gr2 > 0) {
        try { check_file(fin_h); }
        catch (const char* msg) { std::cout << msg; return 1; }

        Long_st* s_gr2_from_file = bin_to_str_long(fin_h, size_gr2);
        fin_h.close();

        std::cout << "¬≈ƒŒÃŒ—“‹ √–”œœ€ " << num2 << " (œŒ ”¡€¬¿Õ»ﬁ —–.¡¿ÀÀ¿):\n";
        print_long_st(s_gr2_from_file, size_gr2);

        delete[] s_gr2_from_file;
    }
    else { std::cout << "“¿ Œ… √–”œœ€ Õ≈“\n"; }
    fin_h.close();

    //Á‡‰‡ÌËÂ Ë
    int best_size = return_best_size(long_st, size);
    Short_st* st_best = best_students(long_st, size);

    std::ofstream fout_best("otlichniki.bin", std::ios::binary);
    try { check_open_file(fout_best); }
    catch (const char* msg) { std::cout << msg; return 1; }

    fill_bin_file_bad(fout_best, st_best, best_size);
    fout_best.close();

    std::ifstream fin_best("otlichniki.bin", std::ios::binary);
    if (best_size > 0) { //ÏÓÊÂÚ ‚˚ÍËÌÛÚ¸ Ó¯Ë·ÍÛ ‚ ÔÓÚ. ÒÎÛ˜‡Â
        try { check_file(fin_best); }
        catch (const char* msg) { std::cout << msg; return 1; }

        Short_st* st_best_from_file = bin_to_str_bad(fin_best, best_size);
        fin_best.close();

        std::cout << "—œ»—Œ  Œ“À»◊Õ» Œ¬:\n";
        print_short_st(st_best_from_file, best_size);

        delete[] st_best_from_file;
    }
    else { std::cout << "—œ»—Œ  Œ“À»◊Õ» Œ¬ œ”—“\n"; }
    fin_best.close();

    //Á‡‰‡ÌËÂ 4
    std::ofstream fout_avg_txt("st_avg_txt.txt");
    try { check_open_file(fout_avg_txt); }
    catch (const char* msg) { std::cout << msg; return 1; }

    fill_txt_long(fout_avg_txt, long_st, size);
    fout_avg_txt.close();

    std::ifstream fin_avg_txt("st_avg_txt.txt");
    try { check_file(fin_avg_txt); }
    catch (const char* msg) { std::cout << msg; return 1; }

    for (int i{}; i < 100; ++i) std::cout << "-";
    std::cout << "\nœ–≈ƒ—“¿¬À≈Õ»≈ “≈ —“Œ¬€’ ‘¿…ÀŒ¬:\n";

    std::cout << "\n—“”ƒ≈Õ“€ —Œ —–≈ƒÕ»Ã» ¡¿ÀÀ¿Ã»:\n";
    print_txt(fin_avg_txt);
    fin_avg_txt.close();

    std::ofstream fout_bad_txt("bad_st_txt.txt");
    try { check_open_file(fout_bad_txt); }
    catch (const char* msg) { std::cout << msg; return 1; }

    fill_txt_short(fout_bad_txt, st_bad_sort, size_bad);
    fout_bad_txt.close();

    std::ifstream fin_bad_txt("bad_st_txt.txt");
    if (size_bad > 0) {
        try { check_file(fin_bad_txt); }
        catch (const char* msg) { std::cout << msg; return 1; }

        std::cout << "\nÕ≈”—œ≈¬¿ﬁŸ»≈, Œ“—Œ–“»–Œ¬¿ÕÕ€≈ œŒ √–”œœ¿Ã » ¿À‘¿¬»“”\n";
        print_txt(fin_bad_txt);
    }
    else { std::cout << "—œ»—Œ  Õ≈”—œ≈¬¿ﬁŸ»’ œ”—“\n"; }
    fin_bad_txt.close();

    std::ofstream out_group_alph_txt("ved1_txt.txt");
    try { check_open_file(out_group_alph_txt); }
    catch (const char* msg) { std::cout << msg; return 1; }

    fill_txt_long(out_group_alph_txt, s_gr1, size_gr1);
    out_group_alph_txt.close();

    std::ifstream in_group_alph_txt("ved1_txt.txt");
    if (size_gr1 > 0) {
        try { check_file(in_group_alph_txt); }
        catch (const char* msg) { std::cout << msg; return 1; }

        std::cout << "\n¬≈ƒŒÃŒ—“‹ √–”œœ€ " << num1 << " (œŒ ¿À‘¿¬»“”, —¬Œﬂ —Œ–“»–Œ¬ ¿):\n";
        print_txt(in_group_alph_txt);
    }
    else { std::cout << "“¿ Œ… √–”œœ€ Õ≈“\n"; }
    in_group_alph_txt.close();

    std::ofstream out_group_avg_txt("ved2_txt.txt");
    try { check_open_file(out_group_avg_txt); }
    catch (const char* msg) { std::cout << msg; return 1; }

    Long_st* s_gr22 = vedomost_group_avg_standart(long_st, size, num2);
    fill_txt_long(out_group_avg_txt, s_gr22, size_gr2);
    out_group_avg_txt.close();

    std::ifstream in_group_avg_txt("ved2_txt.txt");
    if (size_gr2 > 0) {
        try { check_file(in_group_avg_txt); }
        catch (const char* msg) { std::cout << msg; return 1; }

        std::cout << "\n¬≈ƒŒÃŒ—“‹ √–”œœ€ " << num2 << " (œŒ —–.¡¿ÀÀ”, —“¿Õƒ¿–“Õ¿ﬂ —Œ–“»–Œ¬ ¿):\n";
        print_txt(in_group_avg_txt);
    }
    else { std::cout << "“¿ Œ… √–”œœ€ Õ≈“\n"; }
    in_group_avg_txt.close();

    std::ofstream out_best_txt("best_txt.txt");
    try { check_open_file(out_best_txt); }
    catch (const char* msg) { std::cout << msg; return 1; }

    fill_txt_short(out_best_txt, st_best, best_size);
    out_best_txt.close();

    std::ifstream in_best_txt("best_txt.txt");
    if (best_size > 0) {
        try { check_file(in_best_txt); }
        catch (const char* msg) { std::cout << msg; return 1; }

        std::cout << "\n—œ»—Œ  Œ“À»◊Õ» Œ¬:\n";
        print_txt(in_best_txt);
    }
    else { std::cout << "\n—œ»—Œ  Œ“À»◊Õ» Œ¬ œ”—“\n"; }
    in_best_txt.close();

    delete[] st_best;
    delete[] s_gr22;
    delete[] s_gr1;
    delete[] s_gr2;
    delete[] st_bad_sort;
    delete[] stud_name;
    delete[] stud_marks;
    delete[] student;
    delete[] long_st;

    return 0;
}
