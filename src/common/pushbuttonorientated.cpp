/*
Copyright 2010-2013  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#include "pushbuttonorientated.h"

#include <QStylePainter>
#include <QMenu>

PushButtonOrientated::PushButtonOrientated(QWidget* parent)
    : QPushButton(parent)
{
    initObject();
}

PushButtonOrientated::PushButtonOrientated(const QString& text, QWidget* parent)
    : QPushButton(text, parent)
{
    initObject();
}

PushButtonOrientated::PushButtonOrientated(const QIcon& icon, const QString& text, QWidget* parent)
    : QPushButton(icon, text, parent)
{
    initObject();
}

PushButtonOrientated::~PushButtonOrientated()
{
}

void PushButtonOrientated::initObject()
{
    m_orientation = Qt::Horizontal;
    m_mirrored = false;
}

Qt::Orientation PushButtonOrientated::getOrientation() const
{
    return m_orientation;
}

void PushButtonOrientated::setOrientation(Qt::Orientation orientation)
{
    m_orientation = orientation;
    switch (orientation) {
    case Qt::Horizontal:
        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        break;

    case Qt::Vertical:
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        break;
    }
}

bool PushButtonOrientated::mirrored() const
{
    return m_mirrored;
}

void PushButtonOrientated::setMirrored(bool mirrored)
{
    m_mirrored = mirrored;
}

QSize PushButtonOrientated::sizeHint() const
{
    QSize size = QPushButton::sizeHint();

    if (m_orientation == Qt::Vertical) {
        size.transpose();
    }

    return size;
}

QSize PushButtonOrientated::minimumSizeHint() const
{
    QSize size = QPushButton::minimumSizeHint();

    if (m_orientation == Qt::Vertical) {
        size.transpose();
    }

    return size;
}

void PushButtonOrientated::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QStylePainter painter(this);

    switch (m_orientation) {
    case Qt::Horizontal:
        if (m_mirrored) {
            painter.rotate(180);
            painter.translate(-width(), -height());
        }
        break;

    case Qt::Vertical:
        if (m_mirrored) {
            painter.rotate(-90);
            painter.translate(-height(), 0);
        } else {
            painter.rotate(90);
            painter.translate(0, -width());
        }
        break;
    }

    painter.drawControl(QStyle::CE_PushButton, getStyleOption());
}

QStyleOptionButton PushButtonOrientated::getStyleOption() const
{
    QStyleOptionButton option;
    option.initFrom(this);
    if (m_orientation == Qt::Vertical) {
        QSize size = option.rect.size();
        size.transpose();
        option.rect.setSize(size);
    }
    option.features = QStyleOptionButton::None;

    if (isFlat()) {
        option.features |= QStyleOptionButton::Flat;
    }

    if (menu()) {
        option.features |= QStyleOptionButton::HasMenu;
    }

    if (autoDefault() || isDefault()) {
        option.features |= QStyleOptionButton::AutoDefaultButton;
    }

    if (isDefault()) {
        option.features |= QStyleOptionButton::DefaultButton;
    }

    if (isDown() || (menu() && menu()->isVisible())) {
        option.state |= QStyle::State_Sunken;
    }

    if (isChecked()) {
        option.state |= QStyle::State_On;
    }

    if (!isFlat() && !isDown()) {
        option.state |= QStyle::State_Raised;
    }

    option.text = text();
    option.icon = icon();
    option.iconSize = iconSize();
    return option;
}
