/*
Copyright 2011-2012  Francesco Cecconi <francesco.cecconi@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PUSHBUTTONORIENTATED_H
#define PUSHBUTTONORIENTATED_H

#include <QtGui/QPushButton>
#include <QtGui/QStyleOptionButton>

class PushButtonOrientated : public QPushButton
{
public:
    PushButtonOrientated(QWidget* parent = 0);
    explicit PushButtonOrientated(const QString& text, QWidget* parent = 0);
    PushButtonOrientated(const QIcon& icon, const QString& text, QWidget* parent = 0);
    ~PushButtonOrientated();

    Qt::Orientation getOrientation() const;
    void setOrientation(Qt::Orientation orientation);

    bool mirrored() const;
    void setMirrored(bool mirrored);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

protected:
    Qt::Orientation m_orientation;
    bool m_mirrored;
    void paintEvent(QPaintEvent* event);

private:
    QStyleOptionButton getStyleOption() const;
    void initObject();
};

#endif // QPUSHBUTTONORIENTATED_H
