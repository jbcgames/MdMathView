#include <gint/display.h>
#include <gint/keyboard.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_DOC_SIZE 24576
#define MAX_FILES    16
#define SCREEN_W     128
#define SCREEN_H     64
#define CHAR_W       6
#define LINE_H       10

static char *doc_buf = NULL;
static int   doc_len = 0;

static char file_list[MAX_FILES][32];
static int  file_count = 0;

static const char *demo_md =
	"# Demo Markdown + Math\n\n"
	"## Ecuacion Cuadratica\n"
	"La solucion de $ax^2 + bx + c = 0$ es:\n\n"
	"$$x = \\frac{-b \\pm \\sqrt{b^2 - 4ac}}{2a}$$\n\n"
	"---\n\n"
	"## Identidades\n"
	"- Area circulo: $A = \\pi r^2$\n"
	"- Pitagoras: $c = \\sqrt{a^2 + b^2}$\n"
	"- Suma: $\\sum_{i=1}^{n} i = \\frac{n(n+1)}{2}$\n\n"
	"> Pulsa [EXIT] para volver.\n";

static int ends_with_ci(const char *s, const char *ext)
{
	size_t ls = strlen(s), le = strlen(ext);
	if (ls < le) return 0;
	for (size_t i = 0; i < le; i++) {
		if (tolower((unsigned char)s[ls - le + i]) != tolower((unsigned char)ext[i]))
			return 0;
	}
	return 1;
}

/* Normaliza caracteres UTF-8 comunes en español a ASCII legible */
static void normalize_utf8(char *buf, int *len_out)
{
	int r = 0, w = 0;
	int n = *len_out;
	while (r < n) {
		unsigned char c = (unsigned char)buf[r];
		if (c == 0xC3 && r + 1 < n) {
			unsigned char c2 = (unsigned char)buf[r + 1];
			char rep = '?';
			switch (c2) {
				case 0xA1: rep = 'a'; break; /* á */
				case 0xA9: rep = 'e'; break; /* é */
				case 0xAD: rep = 'i'; break; /* í */
				case 0xB3: rep = 'o'; break; /* ó */
				case 0xBA: rep = 'u'; break; /* ú */
				case 0xBC: rep = 'u'; break; /* ü */
				case 0xB1: rep = 'n'; break; /* ñ */
				case 0x81: rep = 'A'; break; /* Á */
				case 0x89: rep = 'E'; break; /* É */
				case 0x8D: rep = 'I'; break; /* Í */
				case 0x93: rep = 'O'; break; /* Ó */
				case 0x9A: rep = 'U'; break; /* Ú */
				case 0x91: rep = 'N'; break; /* Ñ */
			}
			buf[w++] = rep;
			r += 2;
		} else if (c == 0xC2 && r + 1 < n) {
			unsigned char c2 = (unsigned char)buf[r + 1];
			if (c2 == 0xBF) buf[w++] = '?';      /* ¿ */
			else if (c2 == 0xA1) buf[w++] = '!'; /* ¡ */
			else buf[w++] = ' ';
			r += 2;
		} else {
			buf[w++] = (char)c;
			r++;
		}
	}
	buf[w] = '\0';
	*len_out = w;
}

static void scan_files(void)
{
	file_count = 0;
	DIR *d = opendir("/");
	if (!d) return;
	struct dirent *ent;
	while ((ent = readdir(d)) != NULL && file_count < MAX_FILES) {
		if (ends_with_ci(ent->d_name, ".md") || ends_with_ci(ent->d_name, ".txt")) {
			strncpy(file_list[file_count], ent->d_name, 31);
			file_list[file_count][31] = '\0';
			file_count++;
		}
	}
	closedir(d);
}

static int load_file(const char *name)
{
	if (!doc_buf) return 0;
	char path[40];
	snprintf(path, sizeof(path), "/%s", name);
	FILE *f = fopen(path, "rb");
	if (!f) return 0;
	doc_len = (int)fread(doc_buf, 1, MAX_DOC_SIZE - 1, f);
	doc_buf[doc_len] = '\0';
	fclose(f);
	normalize_utf8(doc_buf, &doc_len);
	return 1;
}

static void load_demo(void)
{
	if (!doc_buf) return;
	strncpy(doc_buf, demo_md, MAX_DOC_SIZE - 1);
	doc_buf[MAX_DOC_SIZE - 1] = '\0';
	doc_len = (int)strlen(doc_buf);
}

/* Dibuja símbolos matemáticos vectoriales escalables (altura h) desde 0 */
static int draw_vector_sym(int x, int y, int h, const char *cmd, int draw)
{
	int w = (h * 3) / 4;
	if (w < 5) w = 5;

	if (strcmp(cmd, "\\sum") == 0) {
		if (draw) {
			int mid_y = y + h / 2;
			int mid_x = x + w / 2;
			dline(x, y, x + w - 1, y, C_BLACK);
			dline(x, y, mid_x, mid_y, C_BLACK);
			dline(mid_x, mid_y, x, y + h - 1, C_BLACK);
			dline(x, y + h - 1, x + w - 1, y + h - 1, C_BLACK);
		}
		return w + 2;
	}
	if (strcmp(cmd, "\\int") == 0) {
		int iw = h / 2;
		if (iw < 4) iw = 4;
		if (draw) {
			int cx = x + iw / 2;
			dline(cx + 1, y, cx + 2, y, C_BLACK);
			dline(cx, y + 1, cx, y + h - 2, C_BLACK);
			dline(cx - 2, y + h - 1, cx - 1, y + h - 1, C_BLACK);
		}
		return iw + 2;
	}
	if (strcmp(cmd, "\\pi") == 0) {
		if (draw) {
			int py = y + h / 3;
			dline(x, py, x + w - 1, py, C_BLACK);
			dline(x + 1, py, x + 1, y + h - 1, C_BLACK);
			dline(x + w - 2, py, x + w - 2, y + h - 1, C_BLACK);
		}
		return w + 2;
	}
	if (strcmp(cmd, "\\pm") == 0) {
		if (draw) {
			int my = y + h / 2 - 1;
			int mx = x + w / 2;
			dline(x + 1, my, x + w - 2, my, C_BLACK);
			dline(mx, my - 2, mx, my + 2, C_BLACK);
			dline(x + 1, y + h - 2, x + w - 2, y + h - 2, C_BLACK);
		}
		return w + 2;
	}
	if (strcmp(cmd, "\\infty") == 0) {
		int iw = h + 2;
		if (draw) {
			int cy = y + h / 2;
			int r = h / 4;
			if (r < 2) r = 2;
			dcircle(x + r + 1, cy, r, C_NONE, C_BLACK);
			dcircle(x + iw - r - 1, cy, r, C_NONE, C_BLACK);
		}
		return iw + 2;
	}
	if (strcmp(cmd, "\\chi") == 0) {
		if (draw) {
			dline(x + 1, y + 1, x + w - 2, y + h - 1, C_BLACK);
			dline(x + w - 2, y + 1, x + 1, y + h - 1, C_BLACK);
		}
		return w + 1;
	}
	if (strcmp(cmd, "\\omega") == 0) {
		if (draw) {
			int my = y + h - 2;
			int mx = x + w / 2;
			dline(x, y + 2, x + 1, my, C_BLACK);
			dline(x + 1, my, mx, y + h / 2, C_BLACK);
			dline(mx, y + h / 2, x + w - 2, my, C_BLACK);
			dline(x + w - 2, my, x + w - 1, y + 2, C_BLACK);
		}
		return w + 2;
	}
	if (strcmp(cmd, "\\varepsilon") == 0 || strcmp(cmd, "\\epsilon") == 0) {
		if (draw) {
			int my = y + h / 2;
			dline(x + 1, y + 1, x + w - 1, y + 1, C_BLACK);
			dline(x, y + 2, x, y + h - 2, C_BLACK);
			dline(x + 1, my, x + w - 2, my, C_BLACK);
			dline(x + 1, y + h - 1, x + w - 1, y + h - 1, C_BLACK);
		}
		return w + 1;
	}
	if (strcmp(cmd, "\\nabla") == 0) {
		if (draw) {
			dline(x, y + 1, x + w - 1, y + 1, C_BLACK);
			dline(x, y + 1, x + w / 2, y + h - 1, C_BLACK);
			dline(x + w - 1, y + 1, x + w / 2, y + h - 1, C_BLACK);
		}
		return w + 2;
	}
	if (strcmp(cmd, "\\hbar") == 0) {
		if (draw) {
			int my = y + h / 2;
			dline(x + 1, y, x + 1, y + h - 1, C_BLACK);
			dline(x + 1, my, x + w - 2, my, C_BLACK);
			dline(x + w - 2, my, x + w - 2, y + h - 1, C_BLACK);
			dline(x, y + 2, x + w - 2, y + 1, C_BLACK);
		}
		return w + 1;
	}
	if (strcmp(cmd, "\\lambda") == 0) {
		if (draw) {
			dline(x + 1, y, x + w - 1, y + h - 1, C_BLACK);
			dline(x + w / 2, y + h / 2, x, y + h - 1, C_BLACK);
		}
		return w + 2;
	}
	if (strcmp(cmd, "\\mu") == 0) {
		if (draw) {
			dline(x + 1, y + 2, x + 1, y + h + 1, C_BLACK);
			dline(x + 1, y + h - 2, x + w - 2, y + h - 2, C_BLACK);
			dline(x + w - 2, y + 2, x + w - 2, y + h - 1, C_BLACK);
		}
		return w + 1;
	}
	if (strcmp(cmd, "\\uparrow") == 0) {
		if (draw) {
			int cx = x + 2;
			dline(cx, y, cx, y + h - 1, C_BLACK);
			dline(cx, y, cx - 2, y + 2, C_BLACK);
			dline(cx, y, cx + 2, y + 2, C_BLACK);
		}
		return 6;
	}
	if (strcmp(cmd, "\\downarrow") == 0) {
		if (draw) {
			int cx = x + 2;
			dline(cx, y, cx, y + h - 1, C_BLACK);
			dline(cx, y + h - 1, cx - 2, y + h - 3, C_BLACK);
			dline(cx, y + h - 1, cx + 2, y + h - 3, C_BLACK);
		}
		return 6;
	}
	return 0;
}

static const char *match_latex_sym(const char *p, int *advance, int *is_vec, char *vec_cmd)
{
	const char *vecs[] = {
		"\\sum", "\\int", "\\pi", "\\pm", "\\infty",
		"\\chi", "\\omega", "\\varepsilon", "\\epsilon", "\\nabla",
		"\\hbar", "\\lambda", "\\mu", "\\uparrow", "\\downarrow", NULL
	};
	for (int i = 0; vecs[i]; i++) {
		size_t len = strlen(vecs[i]);
		if (strncmp(p, vecs[i], len) == 0 && !isalpha((unsigned char)p[len])) {
			*advance = (int)len;
			*is_vec = 1;
			strcpy(vec_cmd, vecs[i]);
			return "";
		}
	}
	*is_vec = 0;
	if (p[0] == '\\' && (p[1] == ',' || p[1] == ' ' || p[1] == ';')) {
		*advance = 2;
		return " ";
	}
	struct { const char *cmd; const char *out; } map[] = {
		{"\\times", "x"}, {"\\div", "/"}, {"\\cdot", "*"},
		{"\\leq", "<="}, {"\\geq", ">="}, {"\\neq", "!="}, {"\\approx", "~="},
		{"\\gg", ">>"}, {"\\ll", "<<"}, {"\\sim", "~"}, {"\\propto", "~"},
		{"\\implies", "=>"}, {"\\rightarrow", "->"}, {"\\dots", "..."},
		{"\\alpha", "a"}, {"\\beta", "b"}, {"\\gamma", "y"}, {"\\sigma", "s"},
		{"\\kappa", "k"}, {"\\theta", "th"}, {"\\partial", "d"}, {"\\log", "log"},
		{"\\qquad", "  "}, {"\\quad", " "}, {"\\left", ""}, {"\\right", ""},
		{NULL, NULL}
	};
	for (int i = 0; map[i].cmd; i++) {
		size_t len = strlen(map[i].cmd);
		if (strncmp(p, map[i].cmd, len) == 0 && !isalpha((unsigned char)p[len])) {
			*advance = (int)len;
			return map[i].out;
		}
	}
	return NULL;
}

static int extract_braces(const char *p, char *out, int max_out)
{
	if (*p != '{') return 0;
	int depth = 0, i = 0, o = 0;
	while (p[i]) {
		if (p[i] == '{') {
			if (depth > 0 && o < max_out - 1) out[o++] = '{';
			depth++;
		} else if (p[i] == '}') {
			depth--;
			if (depth == 0) {
				out[o] = '\0';
				return i + 1;
			}
			if (o < max_out - 1) out[o++] = '}';
		} else if (depth > 0 && o < max_out - 1) {
			out[o++] = p[i];
		}
		i++;
	}
	out[o] = '\0';
	return i;
}

static int draw_math_expr(int x, int y, const char *expr, int draw)
{
	int cur_x = x;
	const char *p = expr;
	while (*p) {
		if (strncmp(p, "\\frac", 5) == 0 && p[5] == '{') {
			char num[96], den[96];
			int n1 = extract_braces(p + 5, num, sizeof(num));
			int n2 = extract_braces(p + 5 + n1, den, sizeof(den));
			int wn = draw_math_expr(0, 0, num, 0);
			int wd = draw_math_expr(0, 0, den, 0);
			int w = (wn > wd ? wn : wd) + 4;
			if (draw) {
				draw_math_expr(cur_x + (w - wn) / 2, y - 7, num, 1);
				dline(cur_x, y + 3, cur_x + w - 2, y + 3, C_BLACK);
				draw_math_expr(cur_x + (w - wd) / 2, y + 5, den, 1);
			}
			cur_x += w;
			p += 5 + n1 + n2;
		} else if (strncmp(p, "\\sqrt", 5) == 0 && p[5] == '{') {
			char inner[96];
			int n1 = extract_braces(p + 5, inner, sizeof(inner));
			int wi = draw_math_expr(0, 0, inner, 0);
			if (draw) {
				dline(cur_x, y + 4, cur_x + 2, y + 7, C_BLACK);
				dline(cur_x + 2, y + 7, cur_x + 4, y - 2, C_BLACK);
				dline(cur_x + 4, y - 2, cur_x + 5 + wi, y - 2, C_BLACK);
				draw_math_expr(cur_x + 5, y + 1, inner, 1);
			}
			cur_x += wi + 6;
			p += 5 + n1;
		} else if (strncmp(p, "\\text", 5) == 0 && p[5] == '{') {
			char txt[64];
			int n1 = extract_braces(p + 5, txt, sizeof(txt));
			if (draw) dprint(cur_x, y, C_BLACK, "%s", txt);
			cur_x += (int)strlen(txt) * CHAR_W;
			p += 5 + n1;
		} else if (strncmp(p, "\\vec", 4) == 0 && p[4] == '{') {
			char inner[16];
			int n1 = extract_braces(p + 4, inner, sizeof(inner));
			int wi = draw_math_expr(0, 0, inner, 0);
			if (draw) {
				draw_math_expr(cur_x, y, inner, 1);
				dline(cur_x, y - 2, cur_x + wi - 1, y - 2, C_BLACK);
				dpixel(cur_x + wi - 2, y - 3, C_BLACK);
			}
			cur_x += wi;
			p += 4 + n1;
		} else if (strncmp(p, "\\tilde", 6) == 0 && p[6] == '{') {
			char inner[16];
			int n1 = extract_braces(p + 6, inner, sizeof(inner));
			int wi = draw_math_expr(0, 0, inner, 0);
			if (draw) {
				draw_math_expr(cur_x, y, inner, 1);
				dline(cur_x, y - 2, cur_x + wi - 1, y - 2, C_BLACK);
			}
			cur_x += wi;
			p += 6 + n1;
		} else if (*p == '^' || *p == '_') {
			int is_sup = (*p == '^');
			char sub[32];
			int adv = 0;
			if (p[1] == '{') {
				adv = 1 + extract_braces(p + 1, sub, sizeof(sub));
			} else if (p[1]) {
				sub[0] = p[1];
				sub[1] = '\0';
				adv = 2;
			} else {
				adv = 1;
				sub[0] = '\0';
			}
			int dy = is_sup ? -3 : 3;
			cur_x += draw_math_expr(cur_x, y + dy, sub, draw);
			p += adv;
		} else if (*p == '\\') {
			int adv = 0, is_vec = 0;
			char vec_cmd[16];
			const char *sym = match_latex_sym(p, &adv, &is_vec, vec_cmd);
			if (is_vec && strcmp(vec_cmd, "\\sum") == 0) {
				p += adv;
				char low[32] = "", up[32] = "";
				for (int k = 0; k < 2; k++) {
					if (*p == '_' && p[1] == '{')
						p += 1 + extract_braces(p + 1, low, sizeof(low));
					else if (*p == '^' && p[1] == '{')
						p += 1 + extract_braces(p + 1, up, sizeof(up));
				}
				int ws = draw_vector_sym(0, 0, 10, "\\sum", 0);
				int wl = low[0] ? draw_math_expr(0, 0, low, 0) : 0;
				int wu = up[0] ? draw_math_expr(0, 0, up, 0) : 0;
				int w = ws;
				if (wl > w) w = wl;
				if (wu > w) w = wu;
				if (draw) {
					if (up[0])  draw_math_expr(cur_x + (w - wu) / 2, y - 8, up, 1);
					draw_vector_sym(cur_x + (w - ws) / 2, y, 10, "\\sum", 1);
					if (low[0]) draw_math_expr(cur_x + (w - wl) / 2, y + 11, low, 1);
				}
				cur_x += w + 2;
			} else if (is_vec) {
				cur_x += draw_vector_sym(cur_x, y, 8, vec_cmd, draw);
				p += adv;
			} else if (sym) {
				int len = (int)strlen(sym);
				if (draw && len > 0) dprint(cur_x, y, C_BLACK, "%s", sym);
				cur_x += len * CHAR_W;
				p += adv;
			} else {
				if (draw) dprint(cur_x, y, C_BLACK, "%c", *p);
				cur_x += CHAR_W;
				p++;
			}
		} else if (*p == '{' || *p == '}') {
			p++;
		} else {
			if (draw) dprint(cur_x, y, C_BLACK, "%c", *p);
			cur_x += CHAR_W;
			p++;
		}
	}
	return cur_x - x;
}

static int is_table_sep(const char *s)
{
	if (*s != '|') return 0;
	while (*s) {
		if (*s != '|' && *s != '-' && *s != ':' && *s != ' ') return 0;
		s++;
	}
	return 1;
}

/* Renderiza texto con soporte inline $...$, ajuste de línea a wrap_w y sin solapamientos */
static void render_inline(const char *s, int start_x, int *y_ptr, int wrap_w, int scroll_x, int draw)
{
	int x = start_x;
	int y = *y_ptr;
	int row_extra_bot = 0;

	while (*s) {
		if (*s == '*' && s[1] == '*') { s += 2; continue; }
		if (*s == '*' || *s == '`') { s++; continue; }

		if (*s == '$') {
			const char *end = strchr(s + 1, '$');
			if (end) {
				char expr[128];
				int elen = (int)(end - (s + 1));
				if (elen >= (int)sizeof(expr)) elen = (int)sizeof(expr) - 1;
				memcpy(expr, s + 1, elen);
				expr[elen] = '\0';

				int is_tall = (strstr(expr, "\\frac") != NULL || strstr(expr, "\\sum") != NULL);
				int w = draw_math_expr(0, 0, expr, 0);

				if (x > start_x && x + w > wrap_w) {
					y += LINE_H + row_extra_bot;
					row_extra_bot = 0;
					x = start_x;
				}
				if (is_tall && row_extra_bot == 0) {
					/* Espacio superior para numerador o límite superior */
					y += 7;
					row_extra_bot = 7;
				}
				if (draw && y > -16 && y < SCREEN_H + 8)
					draw_math_expr(x - scroll_x, y, expr, 1);
				x += w;
				s = end + 1;
				continue;
			}
		}

		/* Salto de línea por palabra completa */
		if (*s != ' ' && (x == start_x || s[-1] == ' ')) {
			int wlen = 0;
			while (s[wlen] && s[wlen] != ' ' && s[wlen] != '$' && s[wlen] != '*') wlen++;
			if (x > start_x && x + wlen * CHAR_W > wrap_w) {
				y += LINE_H + row_extra_bot;
				row_extra_bot = 0;
				x = start_x;
			}
		}
		if (x + CHAR_W > wrap_w) {
			y += LINE_H + row_extra_bot;
			row_extra_bot = 0;
			x = start_x;
		}
		if (draw && y > -10 && y < SCREEN_H)
			dprint(x - scroll_x, y, C_BLACK, "%c", *s);
		x += CHAR_W;
		s++;
	}
	*y_ptr = y + LINE_H + row_extra_bot;
}

/* Renderiza una fila de tabla Markdown con columnas equidistantes y bordes */
static void render_table_row(const char *trim, int *y_ptr, int wrap_w, int scroll_x, int draw)
{
	char cells[6][96];
	int ncols = 0;
	const char *p = trim;
	if (*p == '|') p++;
	while (*p && ncols < 6) {
		const char *bar = strchr(p, '|');
		if (!bar) break;
		const char *a = p, *b = bar;
		while (a < b && *a == ' ') a++;
		while (b > a && b[-1] == ' ') b--;
		int len = (int)(b - a);
		if (len >= 95) len = 95;
		memcpy(cells[ncols], a, len);
		cells[ncols][len] = '\0';
		ncols++;
		p = bar + 1;
	}
	if (ncols == 0) return;

	int col_w = wrap_w / ncols;
	if (col_w < 76) col_w = 76;
	int total_w = col_w * ncols;
	int y = *y_ptr;

	if (draw && y > -12 && y < SCREEN_H) {
		/* Borde superior de la fila */
		dline(2 - scroll_x, y, 2 + total_w - scroll_x, y, C_BLACK);
		/* Barras verticales equidistantes */
		for (int c = 0; c <= ncols; c++) {
			int bx = 2 + c * col_w - scroll_x;
			dline(bx, y, bx, y + 11, C_BLACK);
		}
		/* Contenido de cada celda */
		for (int c = 0; c < ncols; c++) {
			int cx = 4 + c * col_w;
			int cy = y + 2;
			render_inline(cells[c], cx, &cy, cx + col_w - 4, scroll_x, 1);
		}
		/* Borde inferior de la fila */
		dline(2 - scroll_x, y + 11, 2 + total_w - scroll_x, y + 11, C_BLACK);
	}
	*y_ptr = y + 11;
}

static int render_document(int scroll_x, int scroll_y, int wrap_w, int draw)
{
	int y = 2 - scroll_y;
	const char *p = doc_buf;

	while (*p) {
		const char *eol = strchr(p, '\n');
		int len = eol ? (int)(eol - p) : (int)strlen(p);
		if (len > 0 && p[len - 1] == '\r') len--;

		char line[512];
		if (len >= (int)sizeof(line)) len = (int)sizeof(line) - 1;
		memcpy(line, p, len);
		line[len] = '\0';

		const char *trim = line;
		while (*trim == ' ' || *trim == '\t') trim++;

		if (*trim == '\0') {
			y += 5;
		} else if (strncmp(trim, "---", 3) == 0 && (trim[3] == '\0' || trim[3] == '-')) {
			if (draw && y >= 0 && y < SCREEN_H)
				dline(2 - scroll_x, y + 3, wrap_w - scroll_x, y + 3, C_BLACK);
			y += 7;
		} else if (is_table_sep(trim)) {
			/* Separador de tabla omitido porque las filas ya dibujan bordes */
		} else if (trim[0] == '|') {
			render_table_row(trim, &y, wrap_w, scroll_x, draw);
		} else if (trim[0] == '#') {
			int lvl = 0;
			while (trim[lvl] == '#') lvl++;
			const char *txt = trim + lvl;
			while (*txt == ' ') txt++;
			int y0 = y;
			render_inline(txt, 2, &y, wrap_w, scroll_x, draw);
			if (draw && y > 0 && y < SCREEN_H + 10) {
				if (lvl == 1) {
					dline(2 - scroll_x, y0 - 1, wrap_w - scroll_x, y0 - 1, C_BLACK);
					dline(2 - scroll_x, y - 1, wrap_w - scroll_x, y - 1, C_BLACK);
				} else {
					dline(2 - scroll_x, y - 2, wrap_w - scroll_x, y - 2, C_BLACK);
				}
			}
			y += 3;
		} else if (strncmp(trim, "$$", 2) == 0) {
			char expr[256];
			int tlen = (int)strlen(trim);
			int elen = tlen - 2;
			if (elen >= 2 && trim[tlen - 2] == '$' && trim[tlen - 1] == '$') elen -= 2;
			if (elen < 0) elen = 0;
			if (elen >= (int)sizeof(expr)) elen = (int)sizeof(expr) - 1;
			memcpy(expr, trim + 2, elen);
			expr[elen] = '\0';

			int w = draw_math_expr(0, 0, expr, 0);
			int mx = (wrap_w - w) / 2;
			if (mx < 2) mx = 2;
			if (draw && y > -18 && y < SCREEN_H)
				draw_math_expr(mx - scroll_x, y + 7, expr, 1);
			y += 22;
		} else {
			int start_x = 2;
			const char *s = trim;
			if (s[0] == '>' && s[1] == ' ') {
				if (draw && y > -10 && y < SCREEN_H)
					dline(2 - scroll_x, y, 2 - scroll_x, y + 8, C_BLACK);
				start_x = 6;
				s += 2;
			} else if ((s[0] == '-' || s[0] == '*') && s[1] == ' ') {
				if (draw && y > -10 && y < SCREEN_H)
					dprint(2 - scroll_x, y, C_BLACK, "*");
				start_x = 9;
				s += 2;
			}
			render_inline(s, start_x, &y, wrap_w, scroll_x, draw);
		}

		if (!eol) break;
		p = eol + 1;
	}
	return y + scroll_y;
}

static void view_document(void)
{
	int scroll_x = 0;
	int scroll_y = 0;
	int wrap_w = 124;
	int total_h = render_document(0, 0, wrap_w, 0);
	int max_scroll = total_h > SCREEN_H ? total_h - SCREEN_H : 0;

	while (1) {
		dclear(C_WHITE);
		render_document(scroll_x, scroll_y, wrap_w, 1);
		dupdate();

		key_event_t ev = getkey();
		if (ev.key == KEY_EXIT)  break;
		if (ev.key == KEY_UP)    scroll_y -= 10;
		if (ev.key == KEY_DOWN)  scroll_y += 10;
		if (ev.key == KEY_LEFT)  scroll_x -= 12;
		if (ev.key == KEY_RIGHT) scroll_x += 12;
		if (ev.key == KEY_F1)    scroll_y -= 48;
		if (ev.key == KEY_F2)    scroll_y += 48;
		if (ev.key == KEY_F3 && wrap_w > 124) {
			wrap_w -= 24;
			total_h = render_document(0, 0, wrap_w, 0);
			max_scroll = total_h > SCREEN_H ? total_h - SCREEN_H : 0;
		}
		if (ev.key == KEY_F4 && wrap_w < 360) {
			wrap_w += 24;
			total_h = render_document(0, 0, wrap_w, 0);
			max_scroll = total_h > SCREEN_H ? total_h - SCREEN_H : 0;
		}
		if (scroll_x < 0) scroll_x = 0;
		if (scroll_x > 300) scroll_x = 300;
		if (scroll_y < 0) scroll_y = 0;
		if (scroll_y > max_scroll) scroll_y = max_scroll;
	}
}

int main(void)
{
	doc_buf = malloc(MAX_DOC_SIZE);
	scan_files();
	int sel = 0;

	while (1) {
		dclear(C_WHITE);
		drect(0, 0, 127, 9, C_BLACK);
		dprint(2, 1, C_WHITE, "MdMathView (%d arch)", file_count);

		int total_items = file_count + 1;
		for (int i = 0; i < total_items && i < 5; i++) {
			int idx = (sel / 5) * 5 + i;
			if (idx >= total_items) break;
			int y = 13 + i * 10;
			const char *name = (idx == file_count) ? "[Demo integrada]" : file_list[idx];
			if (idx == sel) {
				drect(0, y - 1, 127, y + 8, C_BLACK);
				dprint(2, y, C_WHITE, "> %s", name);
			} else {
				dprint(2, y, C_BLACK, "  %s", name);
			}
		}
		dupdate();

		key_event_t ev = getkey();
		if (ev.key == KEY_UP && sel > 0) sel--;
		if (ev.key == KEY_DOWN && sel < total_items - 1) sel++;
		if (ev.key == KEY_EXE) {
			if (sel == file_count) {
				load_demo();
				view_document();
			} else if (load_file(file_list[sel])) {
				view_document();
			}
		}
	}
	return 1;
}
