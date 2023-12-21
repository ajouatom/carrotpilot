#pragma once

#include <string>
#include <vector>

#include <QButtonGroup>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPushButton>

#include "common/params.h"
#include "selfdrive/ui/qt/widgets/input.h"
#include "selfdrive/ui/qt/widgets/toggle.h"

QFrame* horizontal_line(QWidget* parent = nullptr);
class ElidedLabel : public QLabel {
  Q_OBJECT

public:
  explicit ElidedLabel(QWidget *parent = 0);
  explicit ElidedLabel(const QString &text, QWidget *parent = 0);

signals:
  void clicked();

protected:
  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent* event) override;
  void mouseReleaseEvent(QMouseEvent *event) override {
    if (rect().contains(event->pos())) {
      emit clicked();
    }
  }
  QString lastText_, elidedText_;
};


class AbstractControl : public QFrame {
  Q_OBJECT

public:
  void setDescription(const QString &desc) {
    if (description) description->setText(desc);
  }

  void setTitle(const QString &title) {
    title_label->setText(title);
  }

  void setValue(const QString &val) {
    value->setText(val);
  }

  const QString getDescription() {
    return description->text();
  }

  QLabel *icon_label;
  QPixmap icon_pixmap;

public slots:
  void showDescription() {
    description->setVisible(true);
  }

signals:
  void showDescriptionEvent();

protected:
  AbstractControl(const QString &title, const QString &desc = "", const QString &icon = "", QWidget *parent = nullptr);
  void hideEvent(QHideEvent *e) override;

  QHBoxLayout *hlayout;
  QPushButton *title_label;

private:
  ElidedLabel *value;
  QLabel *description = nullptr;
};

// widget to display a value
class LabelControl : public AbstractControl {
  Q_OBJECT

public:
  LabelControl(const QString &title, const QString &text = "", const QString &desc = "", QWidget *parent = nullptr) : AbstractControl(title, desc, "", parent) {
    label.setText(text);
    label.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    hlayout->addWidget(&label);
  }
  void setText(const QString &text) { label.setText(text); }

private:
  ElidedLabel label;
};

// widget for a button with a label
class ButtonControl : public AbstractControl {
  Q_OBJECT

public:
  ButtonControl(const QString &title, const QString &text, const QString &desc = "", QWidget *parent = nullptr);
  inline void setText(const QString &text) { btn.setText(text); }
  inline QString text() const { return btn.text(); }

signals:
  void clicked();

public slots:
  void setEnabled(bool enabled) { btn.setEnabled(enabled); }

private:
  QPushButton btn;
};

class ButtonIconControl : public AbstractControl {
  Q_OBJECT

public:
  ButtonIconControl(const QString &title, const QString &text, const QString &desc = "", const QString &icon = "", QWidget *parent = nullptr);
  inline void setText(const QString &text) { btn.setText(text); }
  inline QString text() const { return btn.text(); }

signals:
  void clicked();

public slots:
  void setEnabled(bool enabled) { btn.setEnabled(enabled); }

private:
  QPushButton btn;
};

class ToggleControl : public AbstractControl {
  Q_OBJECT

public:
  ToggleControl(const QString &title, const QString &desc = "", const QString &icon = "", const bool state = false, QWidget *parent = nullptr) : AbstractControl(title, desc, icon, parent) {
    toggle.setFixedSize(150, 100);
    if (state) {
      toggle.togglePosition();
    }
    hlayout->addWidget(&toggle);
    QObject::connect(&toggle, &Toggle::stateChanged, this, &ToggleControl::toggleFlipped);
  }

  void setVisualOn() {
    toggle.togglePosition();
  }

  void setEnabled(bool enabled) {
    toggle.setEnabled(enabled);
    toggle.update();
  }

signals:
  void toggleFlipped(bool state);

protected:
  Toggle toggle;
};

// widget to toggle params
class ParamControl : public ToggleControl {
  Q_OBJECT

public:
  ParamControl(const QString &param, const QString &title, const QString &desc, const QString &icon, QWidget *parent = nullptr);
  void setConfirmation(bool _confirm, bool _store_confirm) {
    confirm = _confirm;
    store_confirm = _store_confirm;
  }

  void setActiveIcon(const QString &icon) {
    active_icon_pixmap = QPixmap(icon).scaledToWidth(80, Qt::SmoothTransformation);
  }

  void refresh() {
    bool state = params.getBool(key);
    if (state != toggle.on) {
      toggle.togglePosition();
      setIcon(state);
    }
  }

  void showEvent(QShowEvent *event) override {
    refresh();
  }

private:
  void toggleClicked(bool state);
  void setIcon(bool state) {
    if (state && !active_icon_pixmap.isNull()) {
      icon_label->setPixmap(active_icon_pixmap);
    } else if (!icon_pixmap.isNull()) {
      icon_label->setPixmap(icon_pixmap);
    }
  }

  std::string key;
  Params params;
  QPixmap active_icon_pixmap;
  bool confirm = false;
  bool store_confirm = false;
};

class ParamManageControl : public ParamControl {
  Q_OBJECT

public:
  ParamManageControl(const QString &param, const QString &title, const QString &desc, const QString &icon, QWidget *parent = nullptr)
    : ParamControl(param, title, desc, icon, parent),
      key(param.toStdString()),
      manageButton(new ButtonControl(tr(""), tr("MANAGE"), tr(""))) {
    hlayout->insertWidget(hlayout->indexOf(&toggle) - 1, manageButton);

    connect(this, &ToggleControl::toggleFlipped, this, [this](bool state) {
      refresh();
    });

    connect(manageButton, &ButtonControl::clicked, this, &ParamManageControl::manageButtonClicked);
  }

  void refresh() {
    ParamControl::refresh();
    manageButton->setVisible(params.getBool(key));
  }

protected:
  void showEvent(QShowEvent *event) override {
    ParamControl::showEvent(event);
    refresh();
  }

signals:
  void manageButtonClicked();

private:
  std::string key;
  Params params;
  ButtonControl *manageButton;
};

class ParamValueControl : public ParamControl {
  Q_OBJECT

public:
  ParamValueControl(const QString &param, const QString &title, const QString &desc, const QString &icon, 
                    int minimumValue, int maximumValue, const std::function<QString(int)>& labelFormatter, QWidget *parent = nullptr, bool loop = false)
    : ParamControl(param, title, desc, icon, parent), key(param.toStdString()), 
      minimumValue(minimumValue), maximumValue(maximumValue), loop(loop),
      labelGenerator(labelFormatter), valueLabel(new QLabel(this)),
      title(title), description(desc) {
    value = std::clamp(std::stoi(params.get(key)), minimumValue, maximumValue);
    setupLayout();
    updateValueLabel();
  }

  ParamValueControl(const QString &param, const QString &title, const QString &desc, const QString &icon, 
                    int minimumValue, int maximumValue, const std::map<int, QString> &customLabels, QWidget *parent = nullptr, bool loop = true)
    : ParamValueControl(param, title, desc, icon, minimumValue, maximumValue, 
                        [customLabels](int val) {
                          auto it = customLabels.find(val);
                          return it != customLabels.end() ? it->second : QString::number(val);
                        }, parent, loop) {}

signals:
  void valueChanged();

public:
  void refresh() {
    value = std::clamp(std::stoi(params.get(key)), minimumValue, maximumValue);
    updateValueLabel();
  }

  void updateLabelGenerator(int minVal, int maxVal, std::function<QString(int)> newLabelFormatter) {
    minimumValue = minVal;
    maximumValue = maxVal;
    labelGenerator = std::move(newLabelFormatter);
    refresh();
  }

  void updateTitle(const QString &newTitle) {
    title_label->setText(newTitle);
  }

private:
  void setupLayout() {
    hlayout->setContentsMargins(0, 0, 10, 0);
    hlayout->setSpacing(10);
    hlayout->addWidget(valueLabel);
    hlayout->addSpacing(10);
    hlayout->addWidget(createButton("-", [this]{ adjustValue(-1); }));
    hlayout->addSpacing(10);
    hlayout->addWidget(createButton("+", [this]{ adjustValue(1); }));
    toggle.hide();
  }

  QPushButton *createButton(const QString &text, std::function<void()> slot) {
    QPushButton *button = new QPushButton(text, this);
    button->setFixedSize(150, 100);
    button->setStyleSheet(buttonStyle);
    button->setAutoRepeat(true);
    button->setAutoRepeatInterval(150);
    connect(button, &QPushButton::clicked, this, slot);
    return button;
  }

  void adjustValue(int delta) {
    value = loop ? (value + delta > maximumValue ? minimumValue : value + delta < minimumValue ? maximumValue : value + delta)
                 : std::clamp(value + delta, minimumValue, maximumValue);
    updateValueLabel();
    emit valueChanged();
  }

  void updateValueLabel() {
    valueLabel->setText(labelGenerator(value));
    valueLabel->setStyleSheet("QLabel { color: #E0E879; }");
    params.putInt(key, value);
  }

  bool loop;
  int value;
  int minimumValue;
  int maximumValue;
  std::function<QString(int)> labelGenerator;
  std::string key;
  QLabel *valueLabel;
  QString description;
  QString title;
  Params params;

  const QString buttonStyle = R"(
    QPushButton {
      border-radius: 50px;
      font-size: 50px;
      font-weight: 500;
      height: 100px;
      padding: 0 25 0 25;
      color: #E4E4E4;
      background-color: #393939;
    }
    QPushButton:pressed {
      background-color: #4a4a4a;
    }
  )";
};

class ButtonParamControl : public AbstractControl {
  Q_OBJECT
public:
  ButtonParamControl(const QString &param, const QString &title, const QString &desc, const QString &icon,
                     const std::vector<QString> &button_texts, const int minimum_button_width = 225) : AbstractControl(title, desc, icon) {
    const QString style = R"(
      QPushButton {
        border-radius: 50px;
        font-size: 40px;
        font-weight: 500;
        height:100px;
        padding: 0 25 0 25;
        color: #E4E4E4;
        background-color: #393939;
      }
      QPushButton:pressed {
        background-color: #4a4a4a;
      }
      QPushButton:checked:enabled {
        background-color: #33Ab4C;
      }
      QPushButton:disabled {
        color: #33E4E4E4;
      }
    )";
    key = param.toStdString();
    int value = atoi(params.get(key).c_str());

    button_group = new QButtonGroup(this);
    button_group->setExclusive(true);
    for (int i = 0; i < button_texts.size(); i++) {
      QPushButton *button = new QPushButton(button_texts[i], this);
      button->setCheckable(true);
      button->setChecked(i == value);
      button->setStyleSheet(style);
      button->setMinimumWidth(minimum_button_width);
      hlayout->addWidget(button);
      button_group->addButton(button, i);
    }

    QObject::connect(button_group, QOverload<int, bool>::of(&QButtonGroup::buttonToggled), [=](int id, bool checked) {
      if (checked) {
        params.put(key, std::to_string(id));
      }
    });
  }

  void setEnabled(bool enable) {
    for (auto btn : button_group->buttons()) {
      btn->setEnabled(enable);
    }
  }

private:
  std::string key;
  Params params;
  QButtonGroup *button_group;
};

class ListWidget : public QWidget {
  Q_OBJECT
 public:
  explicit ListWidget(QWidget *parent = 0) : QWidget(parent), outer_layout(this) {
    outer_layout.setMargin(0);
    outer_layout.setSpacing(0);
    outer_layout.addLayout(&inner_layout);
    inner_layout.setMargin(0);
    inner_layout.setSpacing(25); // default spacing is 25
    outer_layout.addStretch();
  }
  inline void addItem(QWidget *w) { inner_layout.addWidget(w); }
  inline void addItem(QLayout *layout) { inner_layout.addLayout(layout); }
  inline void setSpacing(int spacing) { inner_layout.setSpacing(spacing); }

private:
  void paintEvent(QPaintEvent *) override {
    QPainter p(this);
    p.setPen(Qt::gray);
    for (int i = 0; i < inner_layout.count() - 1; ++i) {
      QWidget *widget = inner_layout.itemAt(i)->widget();
      if (widget == nullptr || widget->isVisible()) {
        QRect r = inner_layout.itemAt(i)->geometry();
        int bottom = r.bottom() + inner_layout.spacing() / 2;
        p.drawLine(r.left() + 40, bottom, r.right() - 40, bottom);
      }
    }
  }
  QVBoxLayout outer_layout;
  QVBoxLayout inner_layout;
};

// convenience class for wrapping layouts
class LayoutWidget : public QWidget {
  Q_OBJECT

public:
  LayoutWidget(QLayout *l, QWidget *parent = nullptr) : QWidget(parent) {
    setLayout(l);
  }
};
