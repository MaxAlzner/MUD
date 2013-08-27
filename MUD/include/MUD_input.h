#ifndef MUD_INPUT_H_
#define MUD_INPUT_H_

typedef struct MouseState
{
	MouseState();
	int x;
	int y;
	bool left;
	bool right;
	bool middle;
	bool active;
} MouseState;
typedef struct KeyState
{
	KeyState();
	union
	{
		struct
		{
			bool space, exclamation, quote, pound, dollar, percent, ampersand, apostrophe;
			bool parenthesesLeft, parenthesesRight, asterisk, plus, comma, dash, period, slash;
			bool _0, _1, _2, _3, _4, _5, _6, _7, _8, _9;
			bool colon, semicolon, angleBracketLeft, equal, angleBracketRight, question, atSign;
			bool A, B, C, D, E, F, G, H, I, J, K, L, M;
			bool N, O, P, Q, R, S, T, U, V, W, X, Y, Z;
			bool squareBracketLeft, backslash, squareBracketRight, caret, underscore, backtick;
			bool a, b, c, d, e, f, g, h, i, j, k, l, m;
			bool n, o, p, q, r, s, t, u, v, w, x, y, z;
			bool curlyBracketLeft, verticalBar, curlyBracketRight, tilde, delet;
			bool f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12;
			bool left, up, right, down;
			bool pageUp, pageDown, home, end, insert, escape, backspace;
		} value;
		bool table[128];
	};
} KeyState;

MouseState::MouseState()
{
	memset(this, 0, sizeof(MouseState));
}
KeyState::KeyState()
{
	memset(this, 0, sizeof(KeyState));
}

#endif