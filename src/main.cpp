#define _WIN32_WINNT 0x0501
#include <winsock2.h>
#include <ws2tcpip.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

using std::ostringstream;
using std::string;
using std::vector;

const int MAX_TICKETS = 16;
const int PORT = 8080;

struct Ticket {
    int id;
    char airline[32];
    char origin[8];
    char destination[8];
    char date[16];
    char depart[8];
    char arrive[8];
    int durationMin;
    int price;
    int seats;
};

static Ticket gTickets[MAX_TICKETS];
static int gCount = 0;

static void copyStr(char* dest, size_t n, const char* src) {
    strncpy(dest, src, n - 1);
    dest[n - 1] = '\0';
}

static void addTicket(int id, const char* airline, const char* origin, const char* dest,
                      const char* date, const char* depart, const char* arrive,
                      int durationMin, int price, int seats) {
    Ticket* t = &gTickets[gCount++];
    t->id = id;
    copyStr(t->airline, sizeof(t->airline), airline);
    copyStr(t->origin, sizeof(t->origin), origin);
    copyStr(t->destination, sizeof(t->destination), dest);
    copyStr(t->date, sizeof(t->date), date);
    copyStr(t->depart, sizeof(t->depart), depart);
    copyStr(t->arrive, sizeof(t->arrive), arrive);
    t->durationMin = durationMin;
    t->price = price;
    t->seats = seats;
}

static void seedData() {
    gCount = 0;
    addTicket(1, "Garuda Indonesia", "CGK", "DPS", "20 Okt 2026", "06:15", "09:10", 175, 1850000, 7);
    addTicket(2, "Lion Air", "CGK", "DPS", "20 Okt 2026", "08:40", "11:35", 175, 980000, 12);
    addTicket(3, "AirAsia", "CGK", "SUB", "20 Okt 2026", "07:05", "08:25", 80, 620000, 9);
    addTicket(4, "Citilink", "CGK", "YIA", "20 Okt 2026", "09:20", "10:25", 65, 540000, 4);
    addTicket(5, "Batik Air", "CGK", "KNO", "20 Okt 2026", "05:50", "08:05", 135, 1420000, 6);
    addTicket(6, "Super Air Jet", "CGK", "UPG", "21 Okt 2026", "11:30", "14:55", 145, 1190000, 15);
    addTicket(7, "Garuda Indonesia", "CGK", "SUB", "21 Okt 2026", "13:10", "14:30", 80, 890000, 3);
    addTicket(8, "Lion Air", "SUB", "DPS", "21 Okt 2026", "15:00", "16:20", 80, 710000, 10);
    addTicket(9, "AirAsia", "CGK", "DPS", "22 Okt 2026", "19:45", "22:40", 175, 870000, 18);
    addTicket(10, "Citilink", "CGK", "BDO", "20 Okt 2026", "06:00", "06:50", 50, 390000, 8);
    addTicket(11, "Batik Air", "JOG", "CGK", "20 Okt 2026", "10:15", "11:20", 65, 760000, 5);
    addTicket(12, "Garuda Indonesia", "CGK", "KNO", "22 Okt 2026", "16:40", "18:55", 135, 1680000, 2);
}

static string formatRupiah(int n) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%d", n);
    string raw = buf;
    string out;
    int cnt = 0;
    for (int i = (int)raw.size() - 1; i >= 0; --i) {
        out.insert(out.begin(), raw[i]);
        cnt++;
        if (cnt % 3 == 0 && i != 0) out.insert(out.begin(), '.');
    }
    return "Rp " + out;
}

static string formatDuration(int minutes) {
    ostringstream oss;
    oss << (minutes / 60) << "j " << (minutes % 60) << "m";
    return oss.str();
}

static void swapTicket(Ticket* a, Ticket* b) {
    Ticket tmp = *a;
    *a = *b;
    *b = tmp;
}

static int fieldValue(const Ticket* t, const string& field) {
    if (field == "duration") return t->durationMin;
    return t->price;
}

static void bubbleSort(Ticket* arr, int n, const string& field, vector<string>& logs) {
    logs.push_back("[Bubble Sort] Mulai. Array of struct diakses lewat pointer Ticket*.");
    logs.push_back("[Bubble Sort] Kunci urut: " + (field == "duration" ? string("durasi terbang") : string("harga tiket")));
    int comparisons = 0;
    int swaps = 0;
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        ostringstream pass;
        pass << "[Pass " << (i + 1) << "] Menyapu elemen 0 .. " << (n - i - 2);
        logs.push_back(pass.str());
        for (int j = 0; j < n - i - 1; ++j) {
            Ticket* left = arr + j;
            Ticket* right = arr + j + 1;
            int a = fieldValue(left, field);
            int b = fieldValue(right, field);
            comparisons++;
            ostringstream step;
            step << "  Bandingkan indeks " << j << " vs " << (j + 1)
                 << " | " << left->airline << " " << left->origin << "-" << left->destination
                 << " (" << (field == "duration" ? formatDuration(a) : formatRupiah(a)) << ")"
                 << "  dengan  "
                 << right->airline << " " << right->origin << "-" << right->destination
                 << " (" << (field == "duration" ? formatDuration(b) : formatRupiah(b)) << ")";
            if (a > b) {
                step << "  -> SWAP";
                swapTicket(left, right);
                swaps++;
                swapped = true;
            } else {
                step << "  -> tetap";
            }
            logs.push_back(step.str());
        }
        if (!swapped) {
            logs.push_back("[Bubble Sort] Sudah terurut lebih awal, berhenti.");
            break;
        }
    }
    ostringstream done;
    done << "[Bubble Sort] Selesai. Perbandingan: " << comparisons << ", swap: " << swaps << ".";
    logs.push_back(done.str());
}

static bool containsIgnoreCase(const char* hay, const string& needle) {
    if (needle.empty()) return true;
    string h = hay;
    string n = needle;
    for (size_t i = 0; i < h.size(); ++i) if (h[i] >= 'a' && h[i] <= 'z') h[i] = (char)(h[i] - 32);
    for (size_t i = 0; i < n.size(); ++i) if (n[i] >= 'a' && n[i] <= 'z') n[i] = (char)(n[i] - 32);
    return h.find(n) != string::npos;
}

static void linearSearch(Ticket* arr, int n, const string& query, vector<int>& hits, vector<string>& logs) {
    logs.push_back("[Linear Search] Mulai. Traversal pointer dari &arr[0] sampai &arr[n-1].");
    logs.push_back("[Linear Search] Kata kunci: \"" + query + "\" (maskapai / asal / tujuan).");
    for (int i = 0; i < n; ++i) {
        Ticket* p = arr + i;
        bool match = containsIgnoreCase(p->airline, query) ||
                     containsIgnoreCase(p->origin, query) ||
                     containsIgnoreCase(p->destination, query);
        ostringstream step;
        step << "  Periksa indeks " << i << " (id=" << p->id << ") "
             << p->airline << " " << p->origin << "->" << p->destination
             << (match ? "  -> COCOK" : "  -> lewati");
        logs.push_back(step.str());
        if (match) hits.push_back(i);
    }
    ostringstream done;
    done << "[Linear Search] Selesai. Ditemukan " << hits.size() << " tiket dari " << n << " data.";
    logs.push_back(done.str());
}

static string htmlEscape(const string& s) {
    string o;
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (c == '&') o += "&amp;";
        else if (c == '<') o += "&lt;";
        else if (c == '>') o += "&gt;";
        else if (c == '"') o += "&quot;";
        else o += c;
    }
    return o;
}

static string urlDecode(const string& s) {
    string o;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '+' ) o += ' ';
        else if (s[i] == '%' && i + 2 < s.size()) {
            char hex[3] = {s[i + 1], s[i + 2], 0};
            o += (char)strtol(hex, 0, 16);
            i += 2;
        } else o += s[i];
    }
    return o;
}

static string queryValue(const string& qs, const string& key) {
    string prefix = key + "=";
    size_t start = 0;
    while (start < qs.size()) {
        size_t amp = qs.find('&', start);
        string part = qs.substr(start, amp == string::npos ? string::npos : amp - start);
        if (part.size() >= prefix.size() && part.compare(0, prefix.size(), prefix) == 0) {
            return urlDecode(part.substr(prefix.size()));
        }
        if (amp == string::npos) break;
        start = amp + 1;
    }
    return "";
}

static string airlineColor(const char* name) {
    if (strstr(name, "Garuda")) return "#004e98";
    if (strstr(name, "Lion")) return "#d90429";
    if (strstr(name, "AirAsia")) return "#e11d48";
    if (strstr(name, "Citilink")) return "#16a34a";
    if (strstr(name, "Batik")) return "#b45309";
    return "#0ea5e9";
}

static string renderPage(const string& query, const string& sortBy) {
    Ticket work[MAX_TICKETS];
    for (int i = 0; i < gCount; ++i) work[i] = gTickets[i];
    Ticket* arr = work;

    vector<string> logs;
    logs.push_back("[Struct] typedef data tiket: id, maskapai, rute, jam, durasi, harga, kursi.");
    logs.push_back("[Array] Ticket tickets[16] diisi 12 penerbangan (contoh data Traveloka-like).");

    vector<int> hits;
    int shown = gCount;
    Ticket filtered[MAX_TICKETS];
    int filteredCount = gCount;

    if (!query.empty()) {
        linearSearch(arr, gCount, query, hits, logs);
        filteredCount = 0;
        for (size_t i = 0; i < hits.size(); ++i) {
            filtered[filteredCount++] = *(arr + hits[i]);
        }
        for (int i = 0; i < filteredCount; ++i) work[i] = filtered[i];
        shown = filteredCount;
    } else {
        logs.push_back("[Linear Search] Belum ada kata kunci. Semua tiket ditampilkan.");
    }

    if (sortBy == "price" || sortBy == "duration") {
        if (shown > 1) bubbleSort(arr, shown, sortBy, logs);
        else logs.push_back("[Bubble Sort] Data kurang dari 2, tidak perlu diurutkan.");
    } else {
        logs.push_back("[Bubble Sort] Mode default: urutan katalog (belum sorting).");
    }

    ostringstream html;
    html <<
        "<!DOCTYPE html><html lang='id'><head><meta charset='utf-8'>"
        "<meta name='viewport' content='width=device-width, initial-scale=1'>"
        "<title>FlyGo - Tiket Pesawat</title><style>"
        ":root{--blue:#0770CD;--blue2:#0a4f8a;--bg:#eef4fb;--card:#fff;--ink:#1b2430;--muted:#5b6b7c;}"
        "*{box-sizing:border-box}body{margin:0;font-family:'Segoe UI',system-ui,sans-serif;background:var(--bg);color:var(--ink)}"
        "header{background:linear-gradient(135deg,var(--blue),var(--blue2));color:#fff;padding:18px 28px;display:flex;justify-content:space-between;align-items:center}"
        ".brand{font-weight:800;font-size:26px;letter-spacing:-.5px}.brand span{color:#ffd166}"
        ".tag{font-size:13px;opacity:.9}"
        ".wrap{max-width:1120px;margin:0 auto;padding:22px}"
        ".search{background:#fff;border-radius:16px;padding:18px;box-shadow:0 8px 24px rgba(7,112,205,.12);display:grid;grid-template-columns:1.4fr .9fr auto;gap:12px;align-items:end}"
        "label{display:block;font-size:12px;color:var(--muted);margin-bottom:6px;font-weight:600}"
        "input,select{width:100%;padding:11px 12px;border:1px solid #d5e2ef;border-radius:10px;font-size:14px}"
        "button,.btn{background:var(--blue);color:#fff;border:0;border-radius:10px;padding:12px 18px;font-weight:700;cursor:pointer;text-decoration:none;display:inline-block}"
        "button:hover{background:#0559a6}"
        ".grid{display:grid;grid-template-columns:1.4fr .9fr;gap:18px;margin-top:18px}"
        ".card{background:var(--card);border-radius:16px;padding:16px 18px;margin-bottom:12px;box-shadow:0 6px 16px rgba(17,24,39,.06);border:1px solid #e7eef6}"
        ".row{display:flex;justify-content:space-between;gap:12px;align-items:center}"
        ".air{display:flex;gap:10px;align-items:center;font-weight:700}"
        ".badge{width:10px;height:10px;border-radius:50%}"
        ".route{display:flex;gap:16px;align-items:center;margin:10px 0}"
        ".time{font-size:22px;font-weight:800}.city{font-size:12px;color:var(--muted)}"
        ".line{flex:1;height:2px;background:#dbe7f3;position:relative}.line:after{content:'';position:absolute;right:-4px;top:-3px;border:5px solid transparent;border-left-color:#9bb8d3}"
        ".price{color:var(--blue);font-size:20px;font-weight:800;text-align:right}"
        ".meta{font-size:12px;color:var(--muted)}"
        ".log{background:#0f172a;color:#d1fae5;border-radius:16px;padding:14px;height:640px;overflow:auto;font-family:Consolas,'Courier New',monospace;font-size:12.5px;line-height:1.45}"
        ".log h3{margin:0 0 10px;color:#93c5fd;font-family:'Segoe UI',sans-serif}"
        ".empty{padding:28px;text-align:center;color:var(--muted)}"
        ".chips{margin-top:10px;display:flex;gap:8px;flex-wrap:wrap}"
        ".chip{background:#e8f3ff;color:#0a4f8a;border-radius:999px;padding:4px 10px;font-size:12px;font-weight:600}"
        "footer{margin:18px 0;color:var(--muted);font-size:13px}"
        "@media(max-width:900px){.search,.grid{grid-template-columns:1fr}.log{height:320px}}"
        "</style></head><body>"
        "<header><div class='brand'>Fly<span>Go</span></div><div class='tag'>Cari tiket pesawat • gaya Traveloka</div></header>"
        "<div class='wrap'><form class='search' method='GET' action='/'>"
        "<div><label>Cari maskapai / kota (Linear Search)</label>"
        "<input name='q' placeholder='Contoh: DPS, Garuda, SUB' value='" << htmlEscape(query) << "'></div>"
        "<div><label>Urutkan (Bubble Sort)</label><select name='sort'>"
        "<option value=''" << (sortBy.empty() ? " selected" : "") << ">Tanpa sorting</option>"
        "<option value='price'" << (sortBy == "price" ? " selected" : "") << ">Harga termurah</option>"
        "<option value='duration'" << (sortBy == "duration" ? " selected" : "") << ">Durasi terpendek</option>"
        "</select></div>"
        "<button type='submit'>Cari tiket</button></form>"
        "<div class='chips'><span class='chip'>Struct Ticket</span><span class='chip'>Array tickets[16]</span>"
        "<span class='chip'>Pointer Ticket*</span><span class='chip'>Bubble Sort</span><span class='chip'>Linear Search</span></div>"
        "<div class='grid'><div>";

    if (shown == 0) {
        html << "<div class='card empty'>Tidak ada tiket yang cocok untuk \"" << htmlEscape(query) << "\".</div>";
    } else {
        html << "<p class='meta'>" << shown << " penerbangan ditampilkan</p>";
        for (int i = 0; i < shown; ++i) {
            Ticket* p = arr + i;
            html << "<article class='card'><div class='row'><div class='air'>"
                 << "<span class='badge' style='background:" << airlineColor(p->airline) << "'></span>"
                 << htmlEscape(p->airline) << "</div><div class='meta'>ID " << p->id
                 << " • sisa " << p->seats << " kursi</div></div>"
                 << "<div class='route'><div><div class='time'>" << p->depart << "</div><div class='city'>"
                 << p->origin << "</div></div><div><div class='line'></div><div class='meta' style='text-align:center'>"
                 << formatDuration(p->durationMin) << " • " << p->date << "</div></div>"
                 << "<div><div class='time'>" << p->arrive << "</div><div class='city'>" << p->destination
                 << "</div></div></div>"
                 << "<div class='row'><div class='meta'>Langsung • " << p->origin << " → " << p->destination
                 << "</div><div class='price'>" << formatRupiah(p->price) << "</div></div></article>";
        }
    }

    html << "</div><aside class='log'><h3>Log algoritma</h3><pre style='margin:0;white-space:pre-wrap'>";
    for (size_t i = 0; i < logs.size(); ++i) {
        html << htmlEscape(logs[i]) << "\n";
    }
    html << "</pre></aside></div>"
         << "<footer>Tugas struktur data: Array + Pointer + Struct. Server C++ lokal di port 8080.</footer>"
         << "</div></body></html>";
    return html.str();
}

static string httpResponse(int code, const string& body) {
    ostringstream oss;
    oss << "HTTP/1.1 " << code << (code == 200 ? " OK" : " Error") << "\r\n"
        << "Content-Type: text/html; charset=utf-8\r\n"
        << "Content-Length: " << body.size() << "\r\n"
        << "Connection: close\r\n\r\n"
        << body;
    return oss.str();
}

static void handleClient(SOCKET client) {
    char buf[4096];
    int n = recv(client, buf, sizeof(buf) - 1, 0);
    if (n <= 0) {
        closesocket(client);
        return;
    }
    buf[n] = 0;
    string req(buf);
    string method, path;
    {
        size_t sp1 = req.find(' ');
        size_t sp2 = req.find(' ', sp1 == string::npos ? 0 : sp1 + 1);
        if (sp1 != string::npos && sp2 != string::npos) {
            method = req.substr(0, sp1);
            path = req.substr(sp1 + 1, sp2 - sp1 - 1);
        }
    }

    string qs;
    size_t qpos = path.find('?');
    string route = path;
    if (qpos != string::npos) {
        qs = path.substr(qpos + 1);
        route = path.substr(0, qpos);
    }

    string body;
    int code = 200;
    if (method == "GET" && (route == "/" || route == "/index.html")) {
        body = renderPage(queryValue(qs, "q"), queryValue(qs, "sort"));
    } else {
        code = 404;
        body = "<h1>404</h1><p>Halaman tidak ditemukan. <a href='/'>Kembali</a></p>";
    }
    string resp = httpResponse(code, body);
    send(client, resp.c_str(), (int)resp.size(), 0);
    closesocket(client);
}

int main() {
    seedData();
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup gagal\n";
        return 1;
    }

    SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server == INVALID_SOCKET) {
        std::cerr << "socket gagal\n";
        return 1;
    }

    int opt = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(server, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "bind gagal (port 8080 mungkin dipakai)\n";
        return 1;
    }
    listen(server, 8);

    std::cout << "FlyGo berjalan di http://127.0.0.1:" << PORT << "\n";
    std::cout << "Tekan Ctrl+C untuk berhenti.\n";

    while (true) {
        SOCKET client = accept(server, 0, 0);
        if (client == INVALID_SOCKET) continue;
        handleClient(client);
    }
}
