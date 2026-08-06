#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alunos.h"
#include "disciplinas.h"
#include "notas.h"
#include "medias.h"

// Conversão de UTF-8 ou ANSI (Windows-1252/CP850) para UTF-16 (Win32 Unicode API)
static const wchar_t* Utf8ToWide(const char* utf8Str) {
    static wchar_t wbufs[8][1024];
    static int bufIdx = 0;
    if (!utf8Str) return L"";
    bufIdx = (bufIdx + 1) % 8;
    // Tenta converter como UTF-8 estrito; se contiver bytes ANSI não-UTF-8, faz fallback para CP_ACP
    if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8Str, -1, wbufs[bufIdx], 1024) == 0) {
        MultiByteToWideChar(CP_ACP, 0, utf8Str, -1, wbufs[bufIdx], 1024);
    }
    return wbufs[bufIdx];
}

// Funções Auxiliares para manipular controles Win32 com suporte total a UTF-8 (acentuação / ç / ã)
static HWND CreateWindowExUTF8(DWORD dwExStyle, const char* lpClassName, const char* lpWindowName,
                               DWORD dwStyle, int X, int Y, int nWidth, int nHeight,
                               HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) {
    return CreateWindowExW(dwExStyle, Utf8ToWide(lpClassName), Utf8ToWide(lpWindowName),
                           dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

static void SetWindowTextUTF8(HWND hwnd, const char* utf8Str) {
    SetWindowTextW(hwnd, Utf8ToWide(utf8Str));
}

static void GetWindowTextUTF8(HWND hwnd, char* outUtf8, int maxLen) {
    wchar_t wbuf[1024];
    GetWindowTextW(hwnd, wbuf, 1024);
    WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, outUtf8, maxLen, NULL, NULL);
}

static int MessageBoxUTF8(HWND hwnd, const char* text, const char* caption, UINT type) {
    return MessageBoxW(hwnd, Utf8ToWide(text), Utf8ToWide(caption), type);
}

static int ListViewInsertColumnUTF8(HWND hwnd, int col, const char* text, int width) {
    LVCOLUMNW lvc;
    ZeroMemory(&lvc, sizeof(lvc));
    lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    lvc.iSubItem = col;
    lvc.pszText = (LPWSTR)Utf8ToWide(text);
    lvc.cx = width;
    return (int)SendMessageW(hwnd, LVM_INSERTCOLUMNW, col, (LPARAM)&lvc);
}

static int ListViewInsertItemUTF8(HWND hwnd, int item, const char* text) {
    LVITEMW lvi;
    ZeroMemory(&lvi, sizeof(lvi));
    lvi.mask = LVIF_TEXT;
    lvi.iItem = item;
    lvi.iSubItem = 0;
    lvi.pszText = (LPWSTR)Utf8ToWide(text);
    return (int)SendMessageW(hwnd, LVM_INSERTITEMW, 0, (LPARAM)&lvi);
}

static void ListViewSetItemTextUTF8(HWND hwnd, int item, int subItem, const char* text) {
    LVITEMW lvi;
    ZeroMemory(&lvi, sizeof(lvi));
    lvi.iSubItem = subItem;
    lvi.pszText = (LPWSTR)Utf8ToWide(text);
    SendMessageW(hwnd, LVM_SETITEMTEXTW, item, (LPARAM)&lvi);
}

// IDs dos Controles da Interface
#define ID_BTN_NAV_ALUNO       1001
#define ID_BTN_NAV_DISCIPLINA  1002
#define ID_BTN_NAV_MATRICULA   1003
#define ID_BTN_NAV_NOTA        1004
#define ID_BTN_NAV_BOLETIM     1005
#define ID_BTN_NAV_LISTADISC   1006
#define ID_BTN_NAV_REM_ALUNO   1007
#define ID_BTN_NAV_REM_DISC    1008
#define ID_BTN_NAV_SALVAR      1009

#define ID_BTN_ACTION_ALUNO    2001
#define ID_BTN_ACTION_DISC     2002
#define ID_BTN_ACTION_MATR     2003
#define ID_BTN_ACTION_NOTA     2004
#define ID_BTN_ACTION_BOL      2005
#define ID_BTN_ACTION_REM_A    2006
#define ID_BTN_ACTION_REM_D    2007

#define ID_LISTVIEW_DATA       4001
#define ID_STATUSBAR           5001

// Dados globais do sistema
static Aluno g_alunos[MAX_ALUNOS];
static Disciplina g_disciplinas[MAX_DISCIPLINAS];
static float g_notas[MAX_ALUNOS][MAX_DISCIPLINAS];
static int g_qtdAlunos = 0;
static int g_qtdDisciplinas = 0;

// Handles da janela principal e controles
static HWND g_hMainWnd = NULL;
static HWND g_hListView = NULL;
static HWND g_hStatusBar = NULL;
static HWND g_hTitleStatic = NULL;

// Inputs dinâmicos (Rótulos e Campos de Texto)
static HWND g_hLbl1 = NULL, g_hEdit1 = NULL;
static HWND g_hLbl2 = NULL, g_hEdit2 = NULL;
static HWND g_hLbl3 = NULL, g_hEdit3 = NULL;
static HWND g_hBtnAction = NULL;

// Pincéis e Fontes
static HFONT g_hFontTitle = NULL;
static HFONT g_hFontBold = NULL;
static HFONT g_hFontNormal = NULL;

static WNDPROC g_oldEditProc = NULL;

// Subclass dos Campos Edit para navegação fluida por ENTER
LRESULT CALLBACK EditSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_CHAR && wParam == VK_RETURN) {
        return 0; // Evita o beep do teclado
    }
    if (msg == WM_KEYDOWN && wParam == VK_RETURN) {
        if (hwnd == g_hEdit1) {
            if (IsWindowVisible(g_hEdit2)) {
                SetFocus(g_hEdit2);
                SendMessage(g_hEdit2, EM_SETSEL, 0, -1);
            } else {
                SendMessage(g_hMainWnd, WM_COMMAND, MAKEWPARAM((WORD)GetWindowLongPtr(g_hBtnAction, GWLP_ID), BN_CLICKED), (LPARAM)g_hBtnAction);
            }
            return 0;
        } else if (hwnd == g_hEdit2) {
            if (IsWindowVisible(g_hEdit3)) {
                SetFocus(g_hEdit3);
                SendMessage(g_hEdit3, EM_SETSEL, 0, -1);
            } else {
                SendMessage(g_hMainWnd, WM_COMMAND, MAKEWPARAM((WORD)GetWindowLongPtr(g_hBtnAction, GWLP_ID), BN_CLICKED), (LPARAM)g_hBtnAction);
            }
            return 0;
        } else if (hwnd == g_hEdit3) {
            SendMessage(g_hMainWnd, WM_COMMAND, MAKEWPARAM((WORD)GetWindowLongPtr(g_hBtnAction, GWLP_ID), BN_CLICKED), (LPARAM)g_hBtnAction);
            return 0;
        }
    }
    return CallWindowProc(g_oldEditProc, hwnd, msg, wParam, lParam);
}

// Protótipos
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
static void CriarInterfaceGrafica(HWND hwnd);
static void CarregarAba(int numAba);
static void AtualizarTabelaAlunos(void);
static void AtualizarTabelaDisciplinas(void);
static void AtualizarTabelaBoletim(int idAluno);
static void SalvarTodosOsDados(void);
static void SetStatusMsg(const char* msg, BOOL eSucesso);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance;
    (void)lpCmdLine;

    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icex);

    inicializarNotas(g_notas);
    carregarAlunos(g_alunos, &g_qtdAlunos);
    carregarDisciplinas(g_disciplinas, &g_qtdDisciplinas);
    carregarNotas(g_notas);

    WNDCLASSEXW wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"SistemaGestaoEscolarGui";

    if (!RegisterClassExW(&wc)) {
        MessageBoxW(NULL, L"Falha ao registrar janela.", L"Erro", MB_ICONERROR);
        return 0;
    }

    g_hFontTitle = CreateFont(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
    g_hFontBold = CreateFont(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
    g_hFontNormal = CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");

    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);
    int winW = 960, winH = 680;
    int posX = (screenW - winW) / 2;
    int posY = (screenH - winH) / 2;

    g_hMainWnd = CreateWindowExW(
        0, L"SistemaGestaoEscolarGui",
        Utf8ToWide("Sistema de Gestão Escolar — Menu de Software (C)"),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        posX, posY, winW, winH,
        NULL, NULL, hInstance, NULL
    );

    if (!g_hMainWnd) return 0;

    ShowWindow(g_hMainWnd, nCmdShow);
    UpdateWindow(g_hMainWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    SalvarTodosOsDados();
    DeleteObject(g_hFontTitle);
    DeleteObject(g_hFontBold);
    DeleteObject(g_hFontNormal);

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            CriarInterfaceGrafica(hwnd);
            CarregarAba(1);
            break;

        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            switch (wmId) {
                case ID_BTN_NAV_ALUNO:      CarregarAba(1); break;
                case ID_BTN_NAV_DISCIPLINA: CarregarAba(2); break;
                case ID_BTN_NAV_MATRICULA:  CarregarAba(3); break;
                case ID_BTN_NAV_NOTA:       CarregarAba(4); break;
                case ID_BTN_NAV_BOLETIM:    CarregarAba(5); break;
                case ID_BTN_NAV_LISTADISC:  CarregarAba(6); break;
                case ID_BTN_NAV_REM_ALUNO:  CarregarAba(7); break;
                case ID_BTN_NAV_REM_DISC:   CarregarAba(8); break;
                case ID_BTN_NAV_SALVAR:
                    SalvarTodosOsDados();
                    MessageBoxUTF8(hwnd, "Dados salvos com sucesso nos arquivos .dat!", "Sucesso", MB_ICONINFORMATION);
                    PostQuitMessage(0);
                    break;

                // Ação: Cadastrar Aluno
                case ID_BTN_ACTION_ALUNO: {
                    char szId[32], szNome[100], szMsg[256];
                    GetWindowTextUTF8(g_hEdit1, szId, sizeof(szId));
                    GetWindowTextUTF8(g_hEdit2, szNome, sizeof(szNome));
                    int id = atoi(szId);

                    if (cadastrarAlunoDados(g_alunos, &g_qtdAlunos, id, szNome, szMsg)) {
                        SetStatusMsg(szMsg, TRUE);
                        SetWindowTextUTF8(g_hEdit1, "");
                        SetWindowTextUTF8(g_hEdit2, "");
                        SetFocus(g_hEdit1);
                        AtualizarTabelaAlunos();
                    } else {
                        SetStatusMsg(szMsg, FALSE);
                    }
                    break;
                }

                // Ação: Cadastrar Disciplina
                case ID_BTN_ACTION_DISC: {
                    char szId[32], szNome[100], szCarga[32], szMsg[256];
                    GetWindowTextUTF8(g_hEdit1, szId, sizeof(szId));
                    GetWindowTextUTF8(g_hEdit2, szNome, sizeof(szNome));
                    GetWindowTextUTF8(g_hEdit3, szCarga, sizeof(szCarga));

                    int id = atoi(szId);
                    int carga = atoi(szCarga);

                    if (cadastrarDisciplinaDados(g_disciplinas, &g_qtdDisciplinas, id, szNome, carga, szMsg)) {
                        SetStatusMsg(szMsg, TRUE);
                        SetWindowTextUTF8(g_hEdit1, "");
                        SetWindowTextUTF8(g_hEdit2, "");
                        SetWindowTextUTF8(g_hEdit3, "");
                        SetFocus(g_hEdit1);
                        AtualizarTabelaDisciplinas();
                    } else {
                        SetStatusMsg(szMsg, FALSE);
                    }
                    break;
                }

                // Ação: Matricular Aluno
                case ID_BTN_ACTION_MATR: {
                    char szIdAluno[32], szIdDisc[32], szMsg[256];
                    GetWindowTextUTF8(g_hEdit1, szIdAluno, sizeof(szIdAluno));
                    GetWindowTextUTF8(g_hEdit2, szIdDisc, sizeof(szIdDisc));

                    int idA = atoi(szIdAluno);
                    int idD = atoi(szIdDisc);

                    if (matricularAlunoDados(g_alunos, g_qtdAlunos, g_disciplinas, g_qtdDisciplinas, g_notas, idA, idD, szMsg)) {
                        SetStatusMsg(szMsg, TRUE);
                        SetWindowTextUTF8(g_hEdit1, "");
                        SetWindowTextUTF8(g_hEdit2, "");
                        SetFocus(g_hEdit1);
                    } else {
                        SetStatusMsg(szMsg, FALSE);
                    }
                    break;
                }

                // Ação: Lançar Nota
                case ID_BTN_ACTION_NOTA: {
                    char szIdAluno[32], szIdDisc[32], szNota[32], szMsg[256];
                    GetWindowTextUTF8(g_hEdit1, szIdAluno, sizeof(szIdAluno));
                    GetWindowTextUTF8(g_hEdit2, szIdDisc, sizeof(szIdDisc));
                    GetWindowTextUTF8(g_hEdit3, szNota, sizeof(szNota));

                    int idA = atoi(szIdAluno);
                    int idD = atoi(szIdDisc);
                    float nota = (float)atof(szNota);

                    if (lancarNotasDados(g_alunos, g_qtdAlunos, g_disciplinas, g_qtdDisciplinas, g_notas, idA, idD, nota, szMsg)) {
                        SetStatusMsg(szMsg, TRUE);
                        SetWindowTextUTF8(g_hEdit1, "");
                        SetWindowTextUTF8(g_hEdit2, "");
                        SetWindowTextUTF8(g_hEdit3, "");
                        SetFocus(g_hEdit1);
                    } else {
                        SetStatusMsg(szMsg, FALSE);
                    }
                    break;
                }

                // Ação: Consultar Boletim
                case ID_BTN_ACTION_BOL: {
                    char szId[32], szMsg[256];
                    GetWindowTextUTF8(g_hEdit1, szId, sizeof(szId));
                    int idA = atoi(szId);

                    BoletimAluno bol;
                    if (gerarBoletimDados(g_alunos, g_qtdAlunos, g_disciplinas, g_qtdDisciplinas, g_notas, idA, &bol, szMsg)) {
                        SetStatusMsg(szMsg, TRUE);
                        AtualizarTabelaBoletim(idA);
                    } else {
                        SetStatusMsg(szMsg, FALSE);
                    }
                    break;
                }

                // Ação: Remover Aluno
                case ID_BTN_ACTION_REM_A: {
                    char szId[32], szMsg[256];
                    GetWindowTextUTF8(g_hEdit1, szId, sizeof(szId));
                    int id = atoi(szId);

                    if (removerAlunoDados(g_alunos, g_qtdAlunos, id, szMsg)) {
                        SetStatusMsg(szMsg, TRUE);
                        SetWindowTextUTF8(g_hEdit1, "");
                        SetFocus(g_hEdit1);
                        AtualizarTabelaAlunos();
                    } else {
                        SetStatusMsg(szMsg, FALSE);
                    }
                    break;
                }

                // Ação: Remover Disciplina
                case ID_BTN_ACTION_REM_D: {
                    char szId[32], szMsg[256];
                    GetWindowTextUTF8(g_hEdit1, szId, sizeof(szId));
                    int id = atoi(szId);

                    if (removerDisciplinaDados(g_disciplinas, g_qtdDisciplinas, id, szMsg)) {
                        SetStatusMsg(szMsg, TRUE);
                        SetWindowTextUTF8(g_hEdit1, "");
                        SetFocus(g_hEdit1);
                        AtualizarTabelaDisciplinas();
                    } else {
                        SetStatusMsg(szMsg, FALSE);
                    }
                    break;
                }
            }
            break;
        }

        case WM_CLOSE:
            SalvarTodosOsDados();
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

static void CriarInterfaceGrafica(HWND hwnd) {
    HWND hHeader = CreateWindowExUTF8(0, "STATIC", "   SISTEMA DE GESTÃO ESCOLAR", WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE, 0, 0, 960, 48, hwnd, NULL, NULL, NULL);
    SendMessage(hHeader, WM_SETFONT, (WPARAM)g_hFontTitle, TRUE);

    int btnY = 60;
    int btnW = 220, btnH = 38;

    HWND hB1 = CreateWindowExUTF8(0, "BUTTON", "1. Cadastrar Aluno", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 15, btnY, btnW, btnH, hwnd, (HMENU)ID_BTN_NAV_ALUNO, NULL, NULL);
    btnY += 44;
    HWND hB2 = CreateWindowExUTF8(0, "BUTTON", "2. Cadastrar Disciplina", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 15, btnY, btnW, btnH, hwnd, (HMENU)ID_BTN_NAV_DISCIPLINA, NULL, NULL);
    btnY += 44;
    HWND hB3 = CreateWindowExUTF8(0, "BUTTON", "3. Matricular Aluno", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 15, btnY, btnW, btnH, hwnd, (HMENU)ID_BTN_NAV_MATRICULA, NULL, NULL);
    btnY += 44;
    HWND hB4 = CreateWindowExUTF8(0, "BUTTON", "4. Lançar/Atualizar Nota", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 15, btnY, btnW, btnH, hwnd, (HMENU)ID_BTN_NAV_NOTA, NULL, NULL);
    btnY += 44;
    HWND hB5 = CreateWindowExUTF8(0, "BUTTON", "5. Listar Boletim", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 15, btnY, btnW, btnH, hwnd, (HMENU)ID_BTN_NAV_BOLETIM, NULL, NULL);
    btnY += 44;
    HWND hB6 = CreateWindowExUTF8(0, "BUTTON", "6. Listar Disciplinas", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 15, btnY, btnW, btnH, hwnd, (HMENU)ID_BTN_NAV_LISTADISC, NULL, NULL);
    btnY += 44;
    HWND hB7 = CreateWindowExUTF8(0, "BUTTON", "7. Remover Aluno", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 15, btnY, btnW, btnH, hwnd, (HMENU)ID_BTN_NAV_REM_ALUNO, NULL, NULL);
    btnY += 44;
    HWND hB8 = CreateWindowExUTF8(0, "BUTTON", "8. Remover Disciplina", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 15, btnY, btnW, btnH, hwnd, (HMENU)ID_BTN_NAV_REM_DISC, NULL, NULL);
    btnY += 50;
    HWND hB9 = CreateWindowExUTF8(0, "BUTTON", "0. Salvar e Sair", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 15, btnY, btnW, btnH, hwnd, (HMENU)ID_BTN_NAV_SALVAR, NULL, NULL);

    SendMessage(hB1, WM_SETFONT, (WPARAM)g_hFontBold, TRUE);
    SendMessage(hB2, WM_SETFONT, (WPARAM)g_hFontBold, TRUE);
    SendMessage(hB3, WM_SETFONT, (WPARAM)g_hFontBold, TRUE);
    SendMessage(hB4, WM_SETFONT, (WPARAM)g_hFontBold, TRUE);
    SendMessage(hB5, WM_SETFONT, (WPARAM)g_hFontBold, TRUE);
    SendMessage(hB6, WM_SETFONT, (WPARAM)g_hFontBold, TRUE);
    SendMessage(hB7, WM_SETFONT, (WPARAM)g_hFontBold, TRUE);
    SendMessage(hB8, WM_SETFONT, (WPARAM)g_hFontBold, TRUE);
    SendMessage(hB9, WM_SETFONT, (WPARAM)g_hFontBold, TRUE);

    g_hTitleStatic = CreateWindowExUTF8(0, "STATIC", "Cadastrar Aluno", WS_CHILD | WS_VISIBLE | SS_LEFT, 255, 60, 670, 30, hwnd, NULL, NULL, NULL);
    SendMessage(g_hTitleStatic, WM_SETFONT, (WPARAM)g_hFontTitle, TRUE);

    g_hLbl1 = CreateWindowExUTF8(0, "STATIC", "ID do Aluno:", WS_CHILD | WS_VISIBLE, 255, 100, 130, 22, hwnd, NULL, NULL, NULL);
    g_hEdit1 = CreateWindowExUTF8(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 390, 98, 180, 26, hwnd, NULL, NULL, NULL);

    g_hLbl2 = CreateWindowExUTF8(0, "STATIC", "Nome Completo:", WS_CHILD | WS_VISIBLE, 255, 135, 130, 22, hwnd, NULL, NULL, NULL);
    g_hEdit2 = CreateWindowExUTF8(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 390, 133, 320, 26, hwnd, NULL, NULL, NULL);

    g_hLbl3 = CreateWindowExUTF8(0, "STATIC", "Carga Horária:", WS_CHILD | WS_VISIBLE, 255, 170, 130, 22, hwnd, NULL, NULL, NULL);
    g_hEdit3 = CreateWindowExUTF8(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 390, 168, 180, 26, hwnd, NULL, NULL, NULL);

    g_hBtnAction = CreateWindowExUTF8(0, "BUTTON", "Cadastrar Aluno", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 730, 131, 190, 32, hwnd, (HMENU)ID_BTN_ACTION_ALUNO, NULL, NULL);

    SendMessage(g_hLbl1, WM_SETFONT, (WPARAM)g_hFontNormal, TRUE);
    SendMessage(g_hLbl2, WM_SETFONT, (WPARAM)g_hFontNormal, TRUE);
    SendMessage(g_hLbl3, WM_SETFONT, (WPARAM)g_hFontNormal, TRUE);
    SendMessage(g_hEdit1, WM_SETFONT, (WPARAM)g_hFontNormal, TRUE);
    SendMessage(g_hEdit2, WM_SETFONT, (WPARAM)g_hFontNormal, TRUE);
    SendMessage(g_hEdit3, WM_SETFONT, (WPARAM)g_hFontNormal, TRUE);
    SendMessage(g_hBtnAction, WM_SETFONT, (WPARAM)g_hFontBold, TRUE);

    // Configurar Subclass dos Edits para aceitar ENTER de forma intuitiva
    g_oldEditProc = (WNDPROC)SetWindowLongPtr(g_hEdit1, GWLP_WNDPROC, (LONG_PTR)EditSubclassProc);
    SetWindowLongPtr(g_hEdit2, GWLP_WNDPROC, (LONG_PTR)EditSubclassProc);
    SetWindowLongPtr(g_hEdit3, GWLP_WNDPROC, (LONG_PTR)EditSubclassProc);

    g_hListView = CreateWindowExW(
        WS_EX_CLIENTEDGE, WC_LISTVIEWW, L"",
        WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL,
        255, 210, 675, 380,
        hwnd, (HMENU)ID_LISTVIEW_DATA, NULL, NULL
    );
    ListView_SetExtendedListViewStyle(g_hListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    SendMessage(g_hListView, WM_SETFONT, (WPARAM)g_hFontNormal, TRUE);

    g_hStatusBar = CreateWindowExW(0, STATUSCLASSNAMEW, Utf8ToWide("Pronto."), WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0, hwnd, (HMENU)ID_STATUSBAR, NULL, NULL);
    SendMessage(g_hStatusBar, WM_SETFONT, (WPARAM)g_hFontNormal, TRUE);
}

static void SetStatusMsg(const char* msg, BOOL eSucesso) {
    (void)eSucesso;
    if (g_hStatusBar) {
        char buffer[512];
        snprintf(buffer, sizeof(buffer), " Status: %s", msg);
        SendMessageW(g_hStatusBar, SB_SETTEXTW, 0, (LPARAM)Utf8ToWide(buffer));
    }
}

static void SalvarTodosOsDados(void) {
    salvarAlunos(g_alunos, g_qtdAlunos);
    salvarDisciplinas(g_disciplinas, g_qtdDisciplinas);
    salvarNotas(g_notas);
}

static void CarregarAba(int numAba) {
    ShowWindow(g_hLbl1, SW_HIDE); ShowWindow(g_hEdit1, SW_HIDE);
    ShowWindow(g_hLbl2, SW_HIDE); ShowWindow(g_hEdit2, SW_HIDE);
    ShowWindow(g_hLbl3, SW_HIDE); ShowWindow(g_hEdit3, SW_HIDE);
    ShowWindow(g_hBtnAction, SW_HIDE);

    switch (numAba) {
        case 1:
            SetWindowTextUTF8(g_hTitleStatic, "1. Cadastrar Novo Aluno");
            SetWindowTextUTF8(g_hLbl1, "ID do Aluno:");
            SetWindowTextUTF8(g_hLbl2, "Nome Completo:");
            SetWindowTextUTF8(g_hBtnAction, "Cadastrar Aluno");
            SetWindowLongPtr(g_hBtnAction, GWLP_ID, ID_BTN_ACTION_ALUNO);

            ShowWindow(g_hLbl1, SW_SHOW); ShowWindow(g_hEdit1, SW_SHOW);
            ShowWindow(g_hLbl2, SW_SHOW); ShowWindow(g_hEdit2, SW_SHOW);
            ShowWindow(g_hBtnAction, SW_SHOW);

            SetFocus(g_hEdit1);
            AtualizarTabelaAlunos();
            SetStatusMsg("Preencha o ID, pressione ENTER para ir ao Nome e ENTER para cadastrar.", TRUE);
            break;

        case 2:
            SetWindowTextUTF8(g_hTitleStatic, "2. Cadastrar Nova Disciplina");
            SetWindowTextUTF8(g_hLbl1, "ID Disciplina:");
            SetWindowTextUTF8(g_hLbl2, "Nome Disciplina:");
            SetWindowTextUTF8(g_hLbl3, "Carga Horária:");
            SetWindowTextUTF8(g_hBtnAction, "Cadastrar Disciplina");
            SetWindowLongPtr(g_hBtnAction, GWLP_ID, ID_BTN_ACTION_DISC);

            ShowWindow(g_hLbl1, SW_SHOW); ShowWindow(g_hEdit1, SW_SHOW);
            ShowWindow(g_hLbl2, SW_SHOW); ShowWindow(g_hEdit2, SW_SHOW);
            ShowWindow(g_hLbl3, SW_SHOW); ShowWindow(g_hEdit3, SW_SHOW);
            ShowWindow(g_hBtnAction, SW_SHOW);

            SetFocus(g_hEdit1);
            AtualizarTabelaDisciplinas();
            SetStatusMsg("Preencha o ID, Nome e Carga Horária. Pressione ENTER para avançar.", TRUE);
            break;

        case 3:
            SetWindowTextUTF8(g_hTitleStatic, "3. Matricular Aluno em Disciplina");
            SetWindowTextUTF8(g_hLbl1, "ID do Aluno:");
            SetWindowTextUTF8(g_hLbl2, "ID da Disciplina:");
            SetWindowTextUTF8(g_hBtnAction, "Efetuar Matrícula");
            SetWindowLongPtr(g_hBtnAction, GWLP_ID, ID_BTN_ACTION_MATR);

            ShowWindow(g_hLbl1, SW_SHOW); ShowWindow(g_hEdit1, SW_SHOW);
            ShowWindow(g_hLbl2, SW_SHOW); ShowWindow(g_hEdit2, SW_SHOW);
            ShowWindow(g_hBtnAction, SW_SHOW);

            SetFocus(g_hEdit1);
            AtualizarTabelaAlunos();
            SetStatusMsg("Informe os IDs do Aluno e da Disciplina.", TRUE);
            break;

        case 4:
            SetWindowTextUTF8(g_hTitleStatic, "4. Lançar ou Atualizar Nota");
            SetWindowTextUTF8(g_hLbl1, "ID do Aluno:");
            SetWindowTextUTF8(g_hLbl2, "ID da Disciplina:");
            SetWindowTextUTF8(g_hLbl3, "Nota (0.0 a 10.0):");
            SetWindowTextUTF8(g_hBtnAction, "Salvar Nota");
            SetWindowLongPtr(g_hBtnAction, GWLP_ID, ID_BTN_ACTION_NOTA);

            ShowWindow(g_hLbl1, SW_SHOW); ShowWindow(g_hEdit1, SW_SHOW);
            ShowWindow(g_hLbl2, SW_SHOW); ShowWindow(g_hEdit2, SW_SHOW);
            ShowWindow(g_hLbl3, SW_SHOW); ShowWindow(g_hEdit3, SW_SHOW);
            ShowWindow(g_hBtnAction, SW_SHOW);

            SetFocus(g_hEdit1);
            AtualizarTabelaAlunos();
            SetStatusMsg("Informe o ID do Aluno, ID da Disciplina e a Nota.", TRUE);
            break;

        case 5:
            SetWindowTextUTF8(g_hTitleStatic, "5. Consultar Boletim do Aluno");
            SetWindowTextUTF8(g_hLbl1, "ID do Aluno:");
            SetWindowTextUTF8(g_hBtnAction, "Gerar Boletim");
            SetWindowLongPtr(g_hBtnAction, GWLP_ID, ID_BTN_ACTION_BOL);

            ShowWindow(g_hLbl1, SW_SHOW); ShowWindow(g_hEdit1, SW_SHOW);
            ShowWindow(g_hBtnAction, SW_SHOW);

            SetFocus(g_hEdit1);
            ListView_DeleteAllItems(g_hListView);
            SetStatusMsg("Digite o ID do Aluno e pressione ENTER para gerar o Boletim.", TRUE);
            break;

        case 6:
            SetWindowTextUTF8(g_hTitleStatic, "6. Lista de Disciplinas Cadastradas");
            AtualizarTabelaDisciplinas();
            SetStatusMsg("Lista completa de disciplinas cadastradas no sistema.", TRUE);
            break;

        case 7:
            SetWindowTextUTF8(g_hTitleStatic, "7. Remover Aluno da Memória");
            SetWindowTextUTF8(g_hLbl1, "ID do Aluno:");
            SetWindowTextUTF8(g_hBtnAction, "Remover Aluno");
            SetWindowLongPtr(g_hBtnAction, GWLP_ID, ID_BTN_ACTION_REM_A);

            ShowWindow(g_hLbl1, SW_SHOW); ShowWindow(g_hEdit1, SW_SHOW);
            ShowWindow(g_hBtnAction, SW_SHOW);

            SetFocus(g_hEdit1);
            AtualizarTabelaAlunos();
            SetStatusMsg("Digite o ID do aluno e pressione ENTER para desativar/remover.", TRUE);
            break;

        case 8:
            SetWindowTextUTF8(g_hTitleStatic, "8. Remover Disciplina da Memória");
            SetWindowTextUTF8(g_hLbl1, "ID Disciplina:");
            SetWindowTextUTF8(g_hBtnAction, "Remover Disciplina");
            SetWindowLongPtr(g_hBtnAction, GWLP_ID, ID_BTN_ACTION_REM_D);

            ShowWindow(g_hLbl1, SW_SHOW); ShowWindow(g_hEdit1, SW_SHOW);
            ShowWindow(g_hBtnAction, SW_SHOW);

            SetFocus(g_hEdit1);
            AtualizarTabelaDisciplinas();
            SetStatusMsg("Digite o ID da disciplina e pressione ENTER para desativar/remover.", TRUE);
            break;
    }
}

static void AtualizarTabelaAlunos(void) {
    ListView_DeleteAllItems(g_hListView);
    while (ListView_DeleteColumn(g_hListView, 0));

    ListViewInsertColumnUTF8(g_hListView, 0, "ID Aluno", 100);
    ListViewInsertColumnUTF8(g_hListView, 1, "Nome Completo do Aluno", 420);
    ListViewInsertColumnUTF8(g_hListView, 2, "Status", 130);

    int pos = 0;
    for (int i = 0; i < g_qtdAlunos; i++) {
        if (g_alunos[i].ativo) {
            char szId[16];
            snprintf(szId, sizeof(szId), "%d", g_alunos[i].id);

            int row = ListViewInsertItemUTF8(g_hListView, pos++, szId);
            ListViewSetItemTextUTF8(g_hListView, row, 1, g_alunos[i].nome);
            ListViewSetItemTextUTF8(g_hListView, row, 2, "Ativo");
        }
    }
}

static void AtualizarTabelaDisciplinas(void) {
    ListView_DeleteAllItems(g_hListView);
    while (ListView_DeleteColumn(g_hListView, 0));

    ListViewInsertColumnUTF8(g_hListView, 0, "ID Disciplina", 120);
    ListViewInsertColumnUTF8(g_hListView, 1, "Nome da Disciplina", 400);
    ListViewInsertColumnUTF8(g_hListView, 2, "Carga Horária", 130);

    int pos = 0;
    for (int i = 0; i < g_qtdDisciplinas; i++) {
        if (g_disciplinas[i].ativo) {
            char szId[16], szCarga[16];
            snprintf(szId, sizeof(szId), "%d", g_disciplinas[i].id);
            snprintf(szCarga, sizeof(szCarga), "%d hrs", g_disciplinas[i].cargaHoraria);

            int row = ListViewInsertItemUTF8(g_hListView, pos++, szId);
            ListViewSetItemTextUTF8(g_hListView, row, 1, g_disciplinas[i].nome);
            ListViewSetItemTextUTF8(g_hListView, row, 2, szCarga);
        }
    }
}

static void AtualizarTabelaBoletim(int idAluno) {
    ListView_DeleteAllItems(g_hListView);
    while (ListView_DeleteColumn(g_hListView, 0));

    ListViewInsertColumnUTF8(g_hListView, 0, "Disciplina / Aluno", 380);
    ListViewInsertColumnUTF8(g_hListView, 1, "Nota Lançada", 130);
    ListViewInsertColumnUTF8(g_hListView, 2, "Situação", 140);

    BoletimAluno bol;
    char msg[256];
    if (gerarBoletimDados(g_alunos, g_qtdAlunos, g_disciplinas, g_qtdDisciplinas, g_notas, idAluno, &bol, msg)) {
        // Título da aba com o Nome do Aluno em destaque
        char szHeaderNome[256];
        snprintf(szHeaderNome, sizeof(szHeaderNome), "Boletim do Aluno: %s (ID: %d)", bol.nomeAluno, bol.idAluno);
        SetWindowTextUTF8(g_hTitleStatic, szHeaderNome);

        // 1ª Linha da Tabela exibe o Nome do Aluno
        char szNomeFormatado[200];
        snprintf(szNomeFormatado, sizeof(szNomeFormatado), "ALUNO: %s (ID: %d)", bol.nomeAluno, bol.idAluno);
        int r0 = ListViewInsertItemUTF8(g_hListView, 0, szNomeFormatado);
        ListViewSetItemTextUTF8(g_hListView, r0, 1, "-");
        ListViewSetItemTextUTF8(g_hListView, r0, 2, "MATRICULADO");

        // Linhas subsequentes com as disciplinas e notas
        for (int i = 0; i < bol.qtdItens; i++) {
            char szNota[16];
            snprintf(szNota, sizeof(szNota), "%.2f", bol.itens[i].nota);

            int row = ListViewInsertItemUTF8(g_hListView, i + 1, bol.itens[i].nomeDisciplina);
            ListViewSetItemTextUTF8(g_hListView, row, 1, szNota);
            ListViewSetItemTextUTF8(g_hListView, row, 2, "-");
        }

        // Linha final com Média Geral e Situação Final
        char szMedia[32];
        snprintf(szMedia, sizeof(szMedia), "%.2f", bol.mediaGeral);

        int rowM = ListViewInsertItemUTF8(g_hListView, bol.qtdItens + 1, "=== MÉDIA GERAL ===");
        ListViewSetItemTextUTF8(g_hListView, rowM, 1, szMedia);
        ListViewSetItemTextUTF8(g_hListView, rowM, 2, bol.aprovado ? "APROVADO" : "REPROVADO");

        char szStatusFormatado[300];
        snprintf(szStatusFormatado, sizeof(szStatusFormatado), "Boletim do aluno '%s' (ID: %d) exibido com sucesso! Média: %.2f", bol.nomeAluno, bol.idAluno, bol.mediaGeral);
        SetStatusMsg(szStatusFormatado, TRUE);
    }
}
