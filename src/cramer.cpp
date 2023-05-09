#include "cramer.h"

#include "math.h"

using matrix = QList<QList<double>>;

static matrix delRowCol(const matrix& a, int deli, int delj) {
    matrix ans;
    ans.reserve(a.size() - 1);
    for (int i = 0; i < a.size(); ++i) {
        if (i == deli)
            continue;
        ans.push_back({});
        ans.back().reserve(a[0].size() - 1);

        for (int j = 0; j < a.size(); ++j) {
            if (j == delj)
                continue;;
            ans.back().push_back(a[i][j]);
        }
    }
    return ans;
}

static double det(const matrix& a) {
    assert(a.size() == a[0].size());
    int n = a.size();
    if (n == 1) {
        return a[0][0];
    }
    double ans = 0;
    for (int j = 0; j < n; ++j) {
        int sgn = j % 2 == 0 ? 1 : -1;
        ans += sgn * a[0][j] * det(delRowCol(a, 0, j));
    }
    return ans;
}

static matrix setColumn(const matrix& a, int j, const QList<double>& col) {
    assert(a.size() == col.size());
    matrix b = a;
    for (int i = 0; i < a.size(); ++i) {
        b[i][j] = col[i];
    }
    return b;
}

QList<double> cramer(const matrix& a, const QList<double>& c) {
    assert(a.size() == a[0].size());
    assert(a.size() == c.size());
    int n = a.size();
    auto d = det(a);
    if (eq(d, 0))
        return {};

    QList<double> ans(n);
    for (int j = 0; j < n; ++j) {
        ans[j] = det(setColumn(a, j, c)) / d;
    }
    return ans;
}

QList<double> cramer(const QList<QList<double>>& b) {
    assert(b.size() + 1 == b[0].size());
    int n = b.size();

    QList<QList<double>> a(n, QList<double>(n));
    QList<double> c(n);

    for (int i = 0; i < n; ++i) {
        c[i] = b[i].back();
        for (int j = 0; j < n; ++j) {
            a[i][j] = b[i][j];
        }
    }

    return cramer(a, c);
}
