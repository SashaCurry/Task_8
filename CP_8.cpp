#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using namespace boost::multiprecision;

class Pattern
{
private:
	static vector <cpp_int> deg2(cpp_int el, cpp_int n) {//Раскладываем число на степени двойки
		vector <cpp_int> res;
		while (n != 0) {
			if (n / el == 1) {
				res.push_back(el);
				n -= el;
				el = 1;
			}
			else
				el *= 2;
		}
		return res;
	}


	static cpp_int multMod(cpp_int n, cpp_int mod, vector <pair <cpp_int, cpp_int>> lst) {//Умножаем число по модулю
		if (lst.size() == 1) {
			cpp_int res = 1;
			for (int i = 0; i < lst[0].second; i++)
				res = res * lst[0].first % mod;
			return res;
		}
		else if (lst[0].second == 1) {
			cpp_int el = lst[0].first;
			lst.erase(lst.begin());
			return (el * multMod(n, mod, lst)) % mod;
		}
		else {
			for (int i = 0; i < lst.size(); i++)
				if (lst[i].second > 1) {
					lst[i].first = (lst[i].first * lst[i].first) % mod;
					lst[i].second /= 2;
				}
			return multMod(n, mod, lst);
		}
	}

public:
	static cpp_int powClosed(cpp_int x, cpp_int y, cpp_int mod) {//Возводим число в степени по модулю
		if (y == 0)
			return 1;

		vector <cpp_int> lst = deg2(1, y);
		vector <pair <cpp_int, cpp_int>> xDegs;
		for (int i = 0; i < lst.size(); i++)
			xDegs.push_back(make_pair(x, lst[i]));

		cpp_int res = multMod(x, mod, xDegs);
		return res;
	}


	static cpp_int funEuler(cpp_int n) {
		cpp_int res = 1;
		for (int i = 2; i < n; i++)
			if (usualEuclid(n, i) == 1)
				res++;
		return res;
	}


	static cpp_int decForm(string x) {
		cpp_int res = 0, deg = 1;
		if (!x.empty() && x.back() == '1')
			res += 1;
		for (short i = x.length() - 2; i >= 0; i--) {
			deg = deg * 2;
			if (x[i] == '1')
				res += deg;
		}
		return res;
	}


	static cpp_int usualEuclid(cpp_int a, cpp_int b) {
		if (a < b)
			swap(a, b);
		if (a < 0 || b < 0)
			throw string{ "Выполнение невозможно: a < 0 или b < 0" };
		else if (b == 0)
			return a;

		cpp_int r = a % b;
		return usualEuclid(b, r);
	}


	static pair <cpp_int, cpp_int> advancedEuclid(cpp_int a, cpp_int b) {
		if (a < 0 || b < 0)
			throw string{ "Выполнение невозможно: a < 0 или b < 0" };

		cpp_int q, aPrev = a, aCur = b, aNext = -1;
		cpp_int xPrev = 1, xCur = 0, xNext;
		cpp_int yPrev = 0, yCur = 1, yNext;
		while (aNext != 0) {
			q = aPrev / aCur;
			aNext = aPrev % aCur;
			aPrev = aCur; aCur = aNext;

			xNext = xPrev - (xCur * q);
			xPrev = xCur; xCur = xNext;

			yNext = yPrev - (yCur * q);
			yPrev = yCur; yCur = yNext;
		}

		return make_pair(xPrev, yPrev);
	}


	static bool miller_rabin(cpp_int n, int k = 10) {
		if (n == 0 || n == 1)
			return false;

		cpp_int d = n - 1;
		cpp_int s = 0;
		while (d % 2 == 0) {
			s++;
			d = d / 2;
		}

		cpp_int nDec = n - 1;
		for (int i = 0; i < k; i++) {
			cpp_int a = rand() % nDec;
			if (a == 0 || a == 1)
				a = a + 2;

			cpp_int x = powClosed(a, d, n);
			if (x == 1 || x == nDec)
				continue;

			bool flag = false;
			for (int j = 0; j < s; j++) {
				x = (x * x) % n;
				if (x == nDec) {
					flag = true;
					break;
				}
			}
			if (!flag)
				return false;
		}

		return true;
	}


	static cpp_int hashStr(string str) {
		while (str.length() < 2)
			str += " ";
		string res = "";
		hash <string> hashStr;

		unsigned short offset = ceil(str.length() / 2.0);
		for (unsigned short i = 0; i < str.length(); i += offset)
			res += to_string(hashStr(str.substr(i, offset)));
		return cpp_int(res);
	}


	static cpp_int generateSimpleNum(unsigned short m) {
		cpp_int q = rand() % 1000;
		while (Pattern::funEuler(q) != q - 1)
			q++;

		cpp_int s, n = 2, nDec;
		while (!Pattern::miller_rabin(n)) {
			string sBin = "1";
			int sBinSize = rand() % (m / 2) + m / 2;
			for (int i = 0; i < sBinSize; i++)
				sBin = sBin + to_string(rand() % 2);
			s = Pattern::decForm(sBin);

			n = (q * s) + 1;
			nDec = n - 1;
		}

		return n;
	}
};



class RSA
{
private:
	cpp_int generateSimpleNum(unsigned short m) {
		cpp_int q = rand() % 1000;
		while (Pattern::funEuler(q) != q - 1)
			q++;

		cpp_int s, n = 2, nDec;
		while (!Pattern::miller_rabin(n)) {
			string sBin = "1";
			int sBinSize = rand() % (m / 2) + m / 2;
			for (int i = 0; i < sBinSize; i++)
				sBin = sBin + to_string(rand() % 2);
			s = Pattern::decForm(sBin);

			n = (q * s) + 1;
			nDec = n - 1;
		}

		return n;
	}
public:
	cpp_int publicKey;
	cpp_int privateKey;
	cpp_int n, phiN;

	RSA(int m)
	{
		cpp_int p = generateSimpleNum(m), q = generateSimpleNum(m);
		this->n = p * q;
		this->phiN = (p - 1) * (q - 1);

		this->publicKey = generateSimpleNum(m / 2);
		this->privateKey = Pattern::advancedEuclid(publicKey, phiN).first;
		while (privateKey < 0)
			privateKey += phiN;
	}


	cpp_int encrypt(cpp_int m)
	{
		if (m < 2 || m > n - 1)
			throw string("Ошибка при шифровании: сообщение должно лежать в промежутке от 2 до " + to_string(this->n));
		return Pattern::powClosed(m, this->publicKey, this->n);
	}


	cpp_int decrypt(cpp_int c)
	{
		return Pattern::powClosed(c, this->privateKey, this->n);
	}
};



void votingHaeSu(int candidates, int countVoters, vector <int> voters)
{
	RSA v(100);
	cout << "\n\n\tРегистратор создаёт ключи vPublic и vPrivate: {" << v.publicKey << ", " << v.privateKey << "}. vPublic выкладывается в открытый доступ";

	set <cpp_int> authorizedKeys;
	map <cpp_int, unsigned short> votes;
	for (unsigned short i = 2; i <= candidates; i++)
		votes.insert(make_pair(i, 0));

	for (unsigned short i = 0; i < voters.size(); i++)
	{
		cout << "\n\n\n\tИзбиратель " << voters[i] << ": ";
		RSA e(100);
		cout << "\n\t\tСоздаёт ключи {ePublic, n} и {ePrivate, n}: {" << e.publicKey << ", " << e.n << "} и {" << e.privateKey << ", " << e.n << "}";

		cpp_int hashEpublic = Pattern::hashStr(to_string(e.publicKey));
		cout << "\n\t\tВычисляет хэш-функцию h(ePublic): " << hashEpublic;

		cpp_int x = Pattern::generateSimpleNum(50);
		cpp_int f = v.encrypt(x) * hashEpublic;
		cout << "\n\t\tНакладывает маскирующий слой на h(ePublic): f = " << f << "\n\t\tОтправляет маскирующий слой f Регистратору";

		cpp_int g = v.decrypt(f);
		cout << "\n\n\t\tРегистратор дешифрует f: g = " << g << "\n\t\tОтправляет дешифрованное сообщение g Пользователю";

		cpp_int xRev = Pattern::advancedEuclid(x, v.n).first;
		while (xRev < 0)
			xRev += v.n;
		cpp_int eSigPub = g * xRev % v.n;
		cout << "\n\n\t\tПользователь снимает маскирующий слой и получает подписанный ключ eSigPub = decrypt(vPrivate, h(ePublic)): " << eSigPub;

		cpp_int leftSide = v.encrypt(eSigPub);
		cout << "\n\t\tПроверяет подлинность подписи Регистратора (encryption(vPublic, eSigPub) = h(ePublic)): " << leftSide << " = " << hashEpublic;
		if (leftSide != hashEpublic)
			throw string("Регистратор не прошёл проверку!");
		cout << "\n\t\tПроверка пройдена. Пользователь отправляет ЦИКу пару {ePublic, eSigPub} = {" << e.publicKey << ", " << eSigPub << "}";

		cout << "\n\n\t\tЦИК также проверяет подлинность подписи Регистратора (encryption(vPublic, eSigPub) = h(ePublic)): " << leftSide << " = " << hashEpublic;
		cout << "\n\t\tПроверяет, совпадет ли хэш-функция от ePublic в паре с той, что хранится в eSigPub: ";
		cout << Pattern::hashStr(to_string(e.publicKey)) << " = " << v.encrypt(eSigPub);
		cout << "\n\t\tПроверки успешно пройдены. ЦИК добавляет авторизированный ключ ePublic в список!";
		authorizedKeys.insert(e.publicKey);

		cpp_int eSecret = Pattern::generateSimpleNum(50);
		cout << "\n\n\t\tПользователь создаёт eSecret для шифрования биллютени: eSecret = " << eSecret;
		cpp_int B = rand() % (candidates - 2) + 2;
		cout << "\n\t\tПодготавливает сообщение B с выбранным решением: B = " << B;

		cpp_int Bsecret = B * eSecret % e.n;
		cpp_int Bsign = e.decrypt(Pattern::hashStr(to_string(Bsecret)));
		cout << "\n\t\tОтправляет ЦИКу тройку {ePublic, encrypt(eSecret, B), sign(ePrivate, h(encrypt(eSecret, B)))}: {";
		cout << e.publicKey << ", " << Bsecret << ", " << Bsign << "}";

		leftSide = Pattern::hashStr(to_string(Bsecret));
		cpp_int rightSide = e.encrypt(Bsign);
		cout << "\n\n\t\tЦИК проверяет подлинность сообщения (h(encrypt(eSecret, B)) = encrypt(ePublic, sign(ePrivate, h(encrypt(eSecret, B)))): ";
		cout << leftSide << " = " << rightSide;
		if (leftSide != rightSide)
		{
			cout << "\nСООБЩЕНИЕ НЕ ПРОШЛО ПРОВЕРКУ!";
			continue;
		}
		cout << "\n\t\tПроверка пройдена! Тройка публикуется в открытом списке";

		cpp_int eSigSecret = e.decrypt(Pattern::hashStr(to_string(eSecret)));
		cout << "\n\n\t\tПосле появления тройки в открытом доступе пользователь отправляет ЦИКу новую тройку {ePublic, eSecret, sign(ePrivate, h(eSecret))}: {";
		cout << e.publicKey << ", " << eSecret << ", " << eSigSecret;

		leftSide = Pattern::hashStr(to_string(eSecret));
		rightSide = e.encrypt(eSigSecret);
		cout << "\n\n\t\tЦИК проверяет подлинность (h(eSecret) = encrypt(sign(ePrivate, h(eSecret))): " << leftSide << " = " << rightSide;
		if (leftSide != rightSide)
		{
			cout << "\nСООБЩЕНИЕ НЕ ПРОШЛО ПРОВЕРКУ!";
			continue;
		}

		cpp_int eSecretRev = Pattern::advancedEuclid(eSecret, e.n).first;
		while (eSecretRev < 0)
			eSecretRev += e.n;
		cout << "\neSecretRev = " << eSecretRev;
		cpp_int res = Bsecret * eSecretRev % e.n;
		cout << "\n\t\tРасшифровывает полученную бюллетень (eSecret^-1 * encrypt(eSecret, B)): B = " << res;
		cout << "\n\t\tПубилкует данные и подсчитывает результат.";

		votes[B]++;
	}

	cout << "\n\nПосле голосования: ";
	cout << "\n\tРегистратор публикует список всех зарегистрировашихся избирателей: {";
	for (unsigned short i = 1; i <= countVoters; i++)
	{
		if (i == countVoters)
			cout << i << "}";
		else
			cout << i << ", ";
	}

	cout << "\n\tЦИК публикует список всех авторизованных ключей: {";
	for (auto i = authorizedKeys.begin(); i != authorizedKeys.end(); i++)
	{
		if (i == --authorizedKeys.end())
			cout << *i << "}";
		else
			cout << *i << ", ";
	}

	cout << "\n\tРезультаты голосования: ";
	for (auto i = votes.begin(); i != votes.end(); i++)
		cout << "\n\t\tКандидат " << i->first << " = " << i->second;
}

int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "ru");
	cout << "Протокол тайного голосования Хэ-Су";

	int candidates;
	cout << "\n\tКоличество кандидатов: ";
	cin >> candidates;
	cout << "\tСписок кандидатов: {";
	candidates++;
	for (int i = 2; i <= candidates; i++)
	{
		if (i == candidates)
			cout << i << "}";
		else
			cout << i << ", ";
	}

	int countVoters;
	cout << "\n\n\tКоличество избирателей: ";
	cin >> countVoters;
	cout << "\tСписок избирателей: {";
	for (int i = 1; i <= countVoters; i++)
	{
		if (i == countVoters)
			cout << i << "}";
		else
			cout << i << ", ";
	}

	vector <int> voters;
	string str = "\n\tСписок легитимных избирателей: {";
	for (int i = 1; i <= countVoters; i++)
		if (rand() % 2 == 1)
		{
			voters.push_back(i);
			str += to_string(i) + ", ";
		}
	str.erase(str.length() - 2);
	cout << str << "}";

	votingHaeSu(candidates, countVoters, voters);

	cout << endl;
	return 0;
}