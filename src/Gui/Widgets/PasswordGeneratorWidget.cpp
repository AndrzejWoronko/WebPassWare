#include "PasswordGeneratorWidget.h"
#include "ApplicationSettings.h"

CPasswordGeneratorWidget::CPasswordGeneratorWidget(QWidget* parent) : QWidget(parent),
    m_updatingSpinBox(false), m_generator(new CPasswordGenerator())
{
    initWidget();

    connect(m_password, &CFormTextField::textChanged, this, &CPasswordGeneratorWidget::updateApplyEnabled);
    connect(m_acceptButton, &CButton::clicked, this, &CPasswordGeneratorWidget::emitNewPassword);
    connect(m_acceptButton, &CButton::clicked, this, &CPasswordGeneratorWidget::saveSettings);

    connect(m_sliderLength, &QSlider::valueChanged, this, &CPasswordGeneratorWidget::sliderMoved);
    connect(m_length, QOverload<int>::of(&CFormNumberIntField::valueChanged), this, &CPasswordGeneratorWidget::spinBoxChanged);

    connect(m_checkButtonUpper, &CToolButton::clicked, this, &CPasswordGeneratorWidget::updateGenerator);
    connect(m_checkButtonLower, &CToolButton::clicked, this, &CPasswordGeneratorWidget::updateGenerator);
    connect(m_checkButtonNumbers, &CToolButton::clicked, this, &CPasswordGeneratorWidget::updateGenerator);
    connect(m_checkButtonSpecialChars, &CToolButton::clicked, this, &CPasswordGeneratorWidget::updateGenerator);
    connect(m_checkBoxEnsureEvery, &CFormCheckField::clicked, this, &CPasswordGeneratorWidget::updateGenerator);
    connect(m_checkBoxExcludeAlike, &CFormCheckField::clicked, this, &CPasswordGeneratorWidget::updateGenerator);

    connect(m_generateButton, &CButton::clicked, this, &CPasswordGeneratorWidget::updateGenerator);
    connect(m_generateButton, &CButton::clicked, this, &CPasswordGeneratorWidget::saveSettings);

    loadSettings();
    reset();
    this->setMinimumSize(500, 350);
}

CPasswordGeneratorWidget::~CPasswordGeneratorWidget()
{
}

void CPasswordGeneratorWidget::initWidget()
{
    auto m_VMainLayout = new CVBoxLayout(this);

    auto m_GridLayout = new CGridLayout();
    auto m_HLayout = new CHBoxLayout();

    m_password = new CFormTextField("m_password",tr("Wygenerowane hasło"), "", 255, false, this);
    CForm::setComponents(m_GridLayout, m_password, m_password->getLabel(), 1, 1, 1, 4);
    m_length = new CFormNumberIntField("m_lenght", tr("Długość hasła"), 0, 1, false, QString(), QString(), this);
    CForm::setComponents(m_GridLayout, m_length, m_length->getLabel(), 2, 1, 1, 1);
    m_sliderLength = new QSlider(this);
    m_sliderLength->setMinimum(1);
    m_sliderLength->setMaximum(64);
    m_sliderLength->setOrientation(Qt::Horizontal);
    m_sliderLength->setTickPosition(QSlider::TicksBelow);
    m_sliderLength->setTickInterval(8);
    m_GridLayout->addWidget(m_sliderLength, 2, 2, 1, 3); //Górny Layout


    auto groupBox = new QGroupBox(tr("Typy znaków"), this);
    auto m_VLayout = new CVBoxLayout(groupBox);

    m_checkButtonUpper = new CToolButton(CButtonPrivate(tr("A-Z"), tr("Duże litery A-Z")), groupBox);
    m_checkButtonUpper->setToolButtonStyle(Qt::ToolButtonTextOnly);
    m_checkButtonUpper->setCheckable(true);

    m_checkButtonLower = new CToolButton(CButtonPrivate(tr("a-z"), tr("Małe litery a-z")), groupBox);
    m_checkButtonLower->setToolButtonStyle(Qt::ToolButtonTextOnly);
    m_checkButtonLower->setCheckable(true);

    m_checkButtonNumbers = new CToolButton(CButtonPrivate(tr("0-9"), tr("Cyfry 0-9")), groupBox);
    m_checkButtonNumbers->setToolButtonStyle(Qt::ToolButtonTextOnly);
    m_checkButtonNumbers->setCheckable(true);

    m_checkButtonSpecialChars = new CToolButton(CButtonPrivate(tr("/*_..."), tr("Znaki specjalne")), groupBox);
    m_checkButtonSpecialChars->setToolButtonStyle(Qt::ToolButtonTextOnly);
    m_checkButtonSpecialChars->setCheckable(true);

    auto m_HButtonLayout = new CHBoxLayout();
    m_HButtonLayout->addWidget(m_checkButtonUpper);
    m_HButtonLayout->addWidget(m_checkButtonLower);
    m_HButtonLayout->addWidget(m_checkButtonNumbers);
    m_HButtonLayout->addWidget(m_checkButtonSpecialChars);
    m_HButtonLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    m_VLayout->addLayout(m_HButtonLayout);

    m_checkBoxEnsureEvery = new CFormCheckField("m_checkBoxEnsureEvery", tr("Wyklucz podobnie wyglądające znaki"), false, false, groupBox);
    m_checkBoxExcludeAlike = new CFormCheckField("m_checkBoxExcludeAlike", tr("Zapewnij, że hasło będzie zawierało znaki ze wszystkich grup"), false, false, groupBox);

    //m_VLayout->addWidget(groupBox); //Środkowy layout

    m_VLayout->addWidget(m_checkBoxEnsureEvery);
    m_checkBoxEnsureEvery->setText(tr("Wyklucz podobnie wyglądające znaki"));
    m_VLayout->addWidget(m_checkBoxExcludeAlike);
    m_checkBoxExcludeAlike->setText(tr("Zapewnij, że hasło będzie zawierało znaki ze wszystkich grup"));

    m_acceptButton = new CButton(CButtonPrivate(tr("Akceptuj"), tr("Akceptuj hasło"), ICON("Ok")));
    m_generateButton = new CButton(CButtonPrivate(tr("Generuj"), tr("Generuj nowe hasło"), ICON("Password-reset")));
    m_HLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    m_HLayout->addWidget(m_generateButton); //Dolny layout
    m_HLayout->addWidget(m_acceptButton); //Dolny layout

    m_VMainLayout->addLayout(m_GridLayout);
    m_VMainLayout->addWidget(groupBox);
    m_VMainLayout->addLayout(m_HLayout);
}

void CPasswordGeneratorWidget::loadSettings()
{
    m_checkButtonUpper->setChecked(SETT.getValue(GENERATOR_SETTING_ENTRY_UPPER_CASE, true).toBool());
    m_checkButtonLower->setChecked(SETT.getValue(GENERATOR_SETTING_ENTRY_LOWER_CASE, true).toBool());
    m_checkButtonNumbers->setChecked(SETT.getValue(GENERATOR_SETTING_ENTRY_NUMBERS, true).toBool());
    m_checkButtonSpecialChars->setChecked(SETT.getValue(GENERATOR_SETTING_ENTRY_SPECIAL_CHARS, false).toBool());

    m_checkBoxExcludeAlike->setChecked(SETT.getValue(GENERATOR_SETTING_ENTRY_EXCLUDE_ALIKE, true).toBool());
    m_checkBoxEnsureEvery->setChecked(SETT.getValue(GENERATOR_SETTING_ENTRY_ENSURE_EVERY, true).toBool());

    m_length->setValue(SETT.getValue(GENERATOR_SETTING_ENTRY_LENGTH, 16).toInt());
}

void CPasswordGeneratorWidget::saveSettings()
{
    SETT.setValue(GENERATOR_SETTING_ENTRY_UPPER_CASE, m_checkButtonUpper->isChecked());
    SETT.setValue(GENERATOR_SETTING_ENTRY_LOWER_CASE, m_checkButtonLower->isChecked());
    SETT.setValue(GENERATOR_SETTING_ENTRY_NUMBERS, m_checkButtonNumbers->isChecked());
    SETT.setValue(GENERATOR_SETTING_ENTRY_SPECIAL_CHARS, m_checkButtonSpecialChars->isChecked());

    SETT.setValue(GENERATOR_SETTING_ENTRY_EXCLUDE_ALIKE, m_checkBoxExcludeAlike->isChecked());
    SETT.setValue(GENERATOR_SETTING_ENTRY_ENSURE_EVERY, m_checkBoxEnsureEvery->isChecked());

    SETT.setValue(GENERATOR_SETTING_ENTRY_LENGTH, m_length->getValue().toInt());
}

void CPasswordGeneratorWidget::reset()
{
    m_password->setValue("");
    updateGenerator();
}

void CPasswordGeneratorWidget::regeneratePassword()
{
    if (m_generator->isValid())
    {
        QString password = m_generator->generatePassword();
        m_password->setValue(password);
    }
}

void CPasswordGeneratorWidget::updateApplyEnabled(const QString& password)
{
    m_acceptButton->setEnabled(!password.isEmpty());
}

void CPasswordGeneratorWidget::emitNewPassword()
{
    Q_EMIT newPassword(m_password->getValue().toString());
}

void CPasswordGeneratorWidget::sliderMoved()
{
    if (m_updatingSpinBox)
    {
        return;
    }
    m_length->setValue(m_sliderLength->value());
    updateGenerator();
}

void CPasswordGeneratorWidget::spinBoxChanged()
{
    if (m_updatingSpinBox)
    {
        return;
    }
    // Interlock so that we don't update twice - this causes issues as the spinbox can go higher than slider
    m_updatingSpinBox = true;
    m_sliderLength->setValue(m_length->getValue().toInt());
    m_updatingSpinBox = false;
    updateGenerator();
}

CPasswordGenerator::CharClasses CPasswordGeneratorWidget::charClasses()
{
    CPasswordGenerator::CharClasses classes;

    if (m_checkButtonLower->isChecked())
    {
        classes |= CPasswordGenerator::LowerLetters;
    }

    if (m_checkButtonUpper->isChecked())
    {
        classes |= CPasswordGenerator::UpperLetters;
    }

    if (m_checkButtonNumbers->isChecked())
    {
        classes |= CPasswordGenerator::Numbers;
    }

    if (m_checkButtonSpecialChars->isChecked())
    {
        classes |= CPasswordGenerator::SpecialCharacters;
    }

    return classes;
}

CPasswordGenerator::GeneratorFlags CPasswordGeneratorWidget::generatorFlags()
{
    CPasswordGenerator::GeneratorFlags flags;

    if (m_checkBoxExcludeAlike->isChecked())
    {
        flags |= CPasswordGenerator::ExcludeLookAlike;
    }

    if (m_checkBoxEnsureEvery->isChecked())
    {
        flags |= CPasswordGenerator::CharFromEveryGroup;
    }

    return flags;
}

void CPasswordGeneratorWidget::updateGenerator()
{
    CPasswordGenerator::CharClasses classes = charClasses();
    CPasswordGenerator::GeneratorFlags flags = generatorFlags();

    int minLength = 0;
    if (flags.testFlag(CPasswordGenerator::CharFromEveryGroup))
    {
        if (classes.testFlag(CPasswordGenerator::LowerLetters))
        {
            minLength++;
        }
        if (classes.testFlag(CPasswordGenerator::UpperLetters))
        {
            minLength++;
        }
        if (classes.testFlag(CPasswordGenerator::Numbers))
        {
            minLength++;
        }
        if (classes.testFlag(CPasswordGenerator::SpecialCharacters))
        {
            minLength++;
        }
    }
    minLength = qMax(minLength, 1);

    if (m_length->getValue().toInt() < minLength)
    {
        m_updatingSpinBox = true;
        m_length->setValue(minLength);
        m_sliderLength->setValue(minLength);
        m_updatingSpinBox = false;
    }
    m_length->setMinimum(minLength);
    m_sliderLength->setMinimum(minLength);

    m_generator->setLength(m_length->getValue().toUInt());
    m_generator->setCharClasses(classes);
    m_generator->setFlags(flags);
    regeneratePassword();
}
