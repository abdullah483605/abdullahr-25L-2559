#include "../include/LoginScreen.h"
#include "../include/UIManager.h"
#include "../include/PatientDashboard.h"
#include "../include/DoctorDashboard.h"
#include "../include/AdminDashboard.h"

── Palette 
static const sf::Color COL_DEEP   (15,  52, 96);   // deep navy
static const sf::Color COL_MED    (41, 128,185);   // medical blue
static const sf::Color COL_LIGHT  (52, 152,219);   // bright blue
static const sf::Color COL_WHITE  (255,255,255);
static const sf::Color COL_CARD   (250,252,255);
static const sf::Color COL_GREEN  (39, 174, 96);
static const sf::Color COL_RED    (231, 76,  60);
static const sf::Color COL_GREY   (127,140,141);
static const sf::Color COL_TXT    (44,  62,  80);

const char* ROLE_LABELS[3] = {"Patient","Doctor","Admin"};


LoginScreen::LoginScreen()
    : selectedRole(0), failedAttempts(0), pulseVal(0.f),
      idInput(nullptr), passInput(nullptr),
      loginBtn(nullptr), exitBtn(nullptr),
      errorLabel(nullptr), attemptsLabel(nullptr),
      isRegistering(false), openRegBtn(nullptr),
      regSubmitBtn(nullptr), regCancelBtn(nullptr) {
          for(int i=0;i<6;i++) { regFields[i]=nullptr; regLabels[i]=nullptr; }
}

LoginScreen::~LoginScreen() {
    delete idInput; delete passInput;
    delete loginBtn; delete exitBtn;
    delete errorLabel; delete attemptsLabel;
    delete openRegBtn; delete regSubmitBtn; delete regCancelBtn;
    for(int i=0;i<6;i++) { delete regFields[i]; delete regLabels[i]; }
}

void LoginScreen::init() {
    UIManager* ui   = UIManager::getInstance();
    sf::Font&  font = ui->getFont();
    sf::Vector2u ws = ui->getWindow()->getSize();
    float W = (float)ws.x, H = (float)ws.y;

    // ── Split background ─────────────────────────────────────
    bgLeft.setSize(sf::Vector2f(W*0.42f, H));
    bgLeft.setFillColor(COL_DEEP);

    bgRight.setSize(sf::Vector2f(W*0.58f, H));
    bgRight.setPosition(W*0.42f, 0);
    bgRight.setFillColor(sf::Color(245,248,252));

    //  Decorative circles 
    float cr[4] = {180,120,80,50};
    sf::Color cc[4] = { sf::Color(41,128,185,40), sf::Color(52,152,219,30),
                        sf::Color(255,255,255,15), sf::Color(41,128,185,25) };
    float cx[4]={-40, W*0.15f, W*0.05f, W*0.35f};
    float cy[4]={H*0.7f, H+30, H*0.2f, -30};
    for (int i=0;i<4;i++) {
        decorCircle[i].setRadius(cr[i]);
        decorCircle[i].setFillColor(cc[i]);
        decorCircle[i].setPosition(cx[i], cy[i]);
    }

    // Hospital cross logo 
    float logoX = W*0.21f, logoY = H*0.30f;
    logoBg.setRadius(52);
    logoBg.setFillColor(sf::Color(255,255,255,30));
    logoBg.setOutlineThickness(3);
    logoBg.setOutlineColor(sf::Color(255,255,255,80));
    logoBg.setPosition(logoX-52, logoY-52);

    crossH.setSize(sf::Vector2f(70,22));
    crossH.setFillColor(COL_WHITE);
    crossH.setOrigin(35,11);
    crossH.setPosition(logoX, logoY);

    crossV.setSize(sf::Vector2f(22,70));
    crossV.setFillColor(COL_WHITE);
    crossV.setOrigin(11,35);
    crossV.setPosition(logoX, logoY);

    //  Branding text (left panel)
    brandTitle.setFont(font);
    brandTitle.setString("MediCore");
    brandTitle.setCharacterSize(48);
    brandTitle.setFillColor(COL_WHITE);
    brandTitle.setStyle(sf::Text::Bold);
    sf::FloatRect tr = brandTitle.getLocalBounds();
    brandTitle.setOrigin(tr.left+tr.width/2.f, 0);
    brandTitle.setPosition(W*0.21f, logoY+75);

    brandSubtitle.setFont(font);
    brandSubtitle.setString("Hospital Management System");
    brandSubtitle.setCharacterSize(16);
    brandSubtitle.setFillColor(sf::Color(189,215,238));
    sf::FloatRect sr = brandSubtitle.getLocalBounds();
    brandSubtitle.setOrigin(sr.left+sr.width/2.f,0);
    brandSubtitle.setPosition(W*0.21f, logoY+135);

    tagline.setFont(font);
    tagline.setString("\"Your Health, Our Priority\"");
    tagline.setCharacterSize(14);
    tagline.setFillColor(sf::Color(149,165,166));
    sf::FloatRect tl = tagline.getLocalBounds();
    tagline.setOrigin(tl.left+tl.width/2.f,0);
    tagline.setPosition(W*0.21f, logoY+165);

    // Card (right panel) 
    float cardW=420, cardH=520;
    float cardX = W*0.42f + (W*0.58f - cardW)/2.f;
    float cardY = (H - cardH)/2.f;

    card.setSize(sf::Vector2f(cardW, cardH));
    card.setPosition(cardX, cardY);
    card.setFillColor(COL_WHITE);
    card.setOutlineThickness(1.f);
    card.setOutlineColor(sf::Color(220,230,240));

    cardHeader.setSize(sf::Vector2f(cardW, 70));
    cardHeader.setPosition(cardX, cardY);
    cardHeader.setFillColor(COL_MED);

    cardHeaderText.setFont(font);
    cardHeaderText.setString("Sign In");
    cardHeaderText.setCharacterSize(24);
    cardHeaderText.setFillColor(COL_WHITE);
    sf::FloatRect cht = cardHeaderText.getLocalBounds();
    cardHeaderText.setOrigin(cht.left+cht.width/2.f, cht.top+cht.height/2.f);
    cardHeaderText.setPosition(cardX+cardW/2.f, cardY+35);

    cardSubText.setFont(font);
    cardSubText.setString("Please select your role");
    cardSubText.setCharacterSize(15);
    cardSubText.setFillColor(COL_GREY);
    sf::FloatRect cs = cardSubText.getLocalBounds();
    cardSubText.setOrigin(cs.left+cs.width/2.f,0);
    cardSubText.setPosition(cardX+cardW/2.f, cardY+85);

    //  Role selector pills 
    float pillW=118, pillH=36, pillGap=10;
    float pillStartX = cardX + (cardW - 3*pillW - 2*pillGap)/2.f;
    float pillY = cardY + 115;
    for (int i=0;i<3;i++) {
        rolePill[i].setSize(sf::Vector2f(pillW, pillH));
        rolePill[i].setPosition(pillStartX + i*(pillW+pillGap), pillY);
        rolePill[i].setOutlineThickness(2);

        roleText[i].setFont(font);
        roleText[i].setString(ROLE_LABELS[i]);
        roleText[i].setCharacterSize(16);
        sf::FloatRect rt = roleText[i].getLocalBounds();
        roleText[i].setOrigin(rt.left+rt.width/2.f, rt.top+rt.height/2.f);
        roleText[i].setPosition(pillStartX+i*(pillW+pillGap)+pillW/2.f, pillY+pillH/2.f);
    }

    //  Form inputs 
    float inputX = cardX+30, inputW = cardW-60;

    // ID icon label
    idIconText.setFont(font);
    idIconText.setString("User ID");
    idIconText.setCharacterSize(14);
    idIconText.setFillColor(COL_MED);
    idIconText.setPosition(inputX, cardY+175);

    idInput = new InputBox(inputX, cardY+197, inputW, 44, "Enter your ID...", font, false);

    passIconText.setFont(font);
    passIconText.setString("Password");
    passIconText.setCharacterSize(14);
    passIconText.setFillColor(COL_MED);
    passIconText.setPosition(inputX, cardY+258);

    passInput = new InputBox(inputX, cardY+280, inputW, 44, "Enter password...", font, true);

    errorLabel    = new Label(inputX, cardY+340, "", font, 15, COL_RED);
    attemptsLabel = new Label(inputX, cardY+362, "", font, 13, sf::Color(150,80,80));

    loginBtn = new Button(inputX, cardY+390, inputW, 40, "SIGN IN",
                          font, COL_GREEN, sf::Color(46,204,113), sf::Color(34,153,84));
    
    openRegBtn = new Button(inputX, cardY+440, inputW, 30, "New Patient? Register Here",
                            font, sf::Color(0,0,0,0), sf::Color(52,152,219,80), sf::Color(41,128,185));
    openRegBtn->setTextColor(COL_MED);

    exitBtn  = new Button(inputX, cardY+480, inputW, 28, "Exit Application",
                          font, sf::Color(0,0,0,0), sf::Color(231,76,60,180), sf::Color(192,57,43));
    exitBtn->setTextColor(COL_RED);

    const char* lbls[]={"Patient ID (P###)","Full Name","Age","Gender (M/F)","Contact (11 digits)","Password (min 6)"};
    for(int i=0; i<6; i++) {
        regLabels[i] = new Label(inputX, cardY+75 + i*55, lbls[i], font, 14, COL_MED);
        regFields[i] = new InputBox(inputX, cardY+95 + i*55, inputW, 36, "", font, i==5);
    }
    regSubmitBtn = new Button(inputX, cardY+430, inputW/2 - 5, 40, "Register", font, COL_GREEN, sf::Color(46,204,113), sf::Color(34,153,84));
    regCancelBtn = new Button(inputX + inputW/2 + 5, cardY+430, inputW/2 - 5, 40, "Cancel", font, COL_GREY, sf::Color(149,165,166), sf::Color(127,140,141));

    animClock.restart();
}

// ─────────────────────────────────────────────────────────────
void LoginScreen::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (isRegistering) {
        for(int i=0;i<6;i++) regFields[i]->handleEvent(event, window);
        regSubmitBtn->handleEvent(event, window);
        regCancelBtn->handleEvent(event, window);
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) attemptRegister();
        return;
    }

    // Role pill clicks
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mp((float)event.mouseButton.x, (float)event.mouseButton.y);
        for (int i=0;i<3;i++) {
            if (rolePill[i].getGlobalBounds().contains(mp)) {
                selectedRole = i;
                idInput->clear(); passInput->clear();
                errorLabel->setText(""); attemptsLabel->setText("");
            }
        }
    }

    idInput->handleEvent(event, window);
    passInput->handleEvent(event, window);
    loginBtn->handleEvent(event, window);
    openRegBtn->handleEvent(event, window);
    exitBtn->handleEvent(event, window);

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
        if (idInput->getSelected())   { idInput->setSelected(false); passInput->setSelected(true); }
        else                          { passInput->setSelected(false); idInput->setSelected(true); }
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
        attemptLogin();
}─


void LoginScreen::update(float dt) {
    pulseVal += dt * 1.5f;

    if (isRegistering) {
        for(int i=0;i<6;i++) regFields[i]->update(dt);
        regSubmitBtn->update(dt);
        regCancelBtn->update(dt);
        if (regSubmitBtn->isPressed()) attemptRegister();
        if (regCancelBtn->isPressed()) {
            isRegistering = false;
            errorLabel->setText("");
            errorLabel->setColor(sf::Color(231, 76, 60));
            cardHeaderText.setString("Sign In");
            cardSubText.setString("Please select your role");
        }
        return;
    }

    idInput->update(dt);
    passInput->update(dt);
    loginBtn->update(dt);
    exitBtn->update(dt);

    // Update pill colours
    for (int i=0;i<3;i++) {
        if (i==selectedRole) {
            rolePill[i].setFillColor(COL_MED);
            rolePill[i].setOutlineColor(COL_DEEP);
            roleText[i].setFillColor(COL_WHITE);
        } else {
            rolePill[i].setFillColor(sf::Color(235,240,248));
            rolePill[i].setOutlineColor(sf::Color(200,210,220));
            roleText[i].setFillColor(COL_TXT);
        }
    }

    if (openRegBtn->isPressed()) {
        isRegistering = true;
        errorLabel->setText("");
        cardHeaderText.setString("Register");
        cardSubText.setString("Create a new patient account");
        for(int i=0;i<6;i++) regFields[i]->clear();
    }

    if (loginBtn->isPressed()) attemptLogin();
    if (exitBtn->isPressed())  UIManager::getInstance()->getWindow()->close();
}


void LoginScreen::attemptLogin() {
    if (failedAttempts >= 3) {
        errorLabel->setText("Account LOCKED after 3 attempts!");
        attemptsLabel->setText("Contact Administrator.");
        return;
    }
    UIManager* ui  = UIManager::getInstance();
    const char* id = idInput->getText();
    const char* pw = passInput->getText();

    if (strLen(id)==0 || strLen(pw)==0) {
        errorLabel->setText("Please fill in both fields."); return;
    }

    bool auth=false;
    if (selectedRole==0) {
        Patient* p = ui->getPatients()->findByID(id);
        if (p && p->login(id,pw)) {
            auth=true; ui->login(id,"Patient");
            ui->getFileHandler()->logSecurity("Patient",id,"SUCCESS");
            ui->getSceneManager()->changeScene(new PatientDashboard());
        }
    } else if (selectedRole==1) {
        Doctor* d = ui->getDoctors()->findByID(id);
        if (d && d->login(id,pw)) {
            auth=true; ui->login(id,"Doctor");
            ui->getFileHandler()->logSecurity("Doctor",id,"SUCCESS");
            ui->getSceneManager()->changeScene(new DoctorDashboard());
        }
    } else {
        Admin* a = ui->getAdmins()->findByID(id);
        if (a && a->login(id,pw)) {
            auth=true; ui->login(id,"Admin");
            ui->getFileHandler()->logSecurity("Admin",id,"SUCCESS");
            ui->getSceneManager()->changeScene(new AdminDashboard());
        }
    }

    if (!auth) {
        failedAttempts++;
        errorLabel->setText("Invalid ID or Password.");
        char attStr[60]; strCopy(attStr,"Attempts remaining: ");
        char num[4]; intToChar(num, 3-failedAttempts); strCat(attStr,num);
        attemptsLabel->setText(attStr);
        ui->getFileHandler()->logSecurity("Unknown",id,"FAILED");
        passInput->clear();
    }
}

void LoginScreen::draw(sf::RenderWindow& window) {
    window.draw(bgLeft);
    window.draw(bgRight);

    for (int i=0;i<4;i++) window.draw(decorCircle[i]);

    window.draw(logoBg);
    window.draw(crossH);
    window.draw(crossV);
    window.draw(brandTitle);
    window.draw(brandSubtitle);
    window.draw(tagline);

    // Card shadow (offset rect)
    sf::RectangleShape shadow = card;
    shadow.move(4,5);
    shadow.setFillColor(sf::Color(0,0,0,25));
    window.draw(shadow);

    window.draw(card);
    window.draw(cardHeader);
    window.draw(cardHeaderText);
    window.draw(cardSubText);

    if (isRegistering) {
        for(int i=0;i<6;i++) { regLabels[i]->draw(window); regFields[i]->draw(window); }
        regSubmitBtn->draw(window);
        regCancelBtn->draw(window);
        errorLabel->draw(window);
    } else {
        for (int i=0;i<3;i++) { window.draw(rolePill[i]); window.draw(roleText[i]); }
        window.draw(idIconText);
        idInput->draw(window);
        window.draw(passIconText);
        passInput->draw(window);
        attemptsLabel->draw(window);
        openRegBtn->draw(window);
        loginBtn->draw(window);
        exitBtn->draw(window);
        errorLabel->draw(window);
    }
}

void LoginScreen::attemptRegister() {
    UIManager* ui = UIManager::getInstance();
    const char* pid = regFields[0]->getText();
    const char* nm  = regFields[1]->getText();
    int age         = charToInt(regFields[2]->getText());
    const char* gStr= regFields[3]->getText();
    char gen        = strLen(gStr)>0 ? gStr[0] : ' ';
    const char* cont= regFields[4]->getText();
    const char* pass= regFields[5]->getText();

    errorLabel->setColor(sf::Color(231, 76, 60));

    if (!ui->getValidator()->validateID(pid,"P"))         { errorLabel->setText("Invalid ID (e.g. P001)"); return; }
    if (strLen(nm)==0)                                    { errorLabel->setText("Name cannot be empty."); return; }
    if (age <= 0 || age > 150)                            { errorLabel->setText("Invalid age."); return; }
    if (gen != 'M' && gen != 'F' && gen != 'm' && gen != 'f') { errorLabel->setText("Gender must be M or F."); return; }
    if (!ui->getValidator()->validateContact(cont))       { errorLabel->setText("Contact must be 11 digits."); return; }
    if (!ui->getValidator()->validatePassword(pass))      { errorLabel->setText("Password must be at least 6 chars."); return; }
    if (ui->getPatients()->findByID(pid))                 { errorLabel->setText("Patient ID already exists."); return; }

    if (gen == 'm') gen = 'M';
    if (gen == 'f') gen = 'F';

    Patient newPat(pid, nm, age, gen, cont, pass, 0.0f);
    ui->getPatients()->add(newPat);
    char csv[512]; newPat.toCSV(csv);
    ui->getFileHandler()->appendRecord("patients.txt", csv);
    
    isRegistering = false;
    errorLabel->setText("Registration Successful! Please login.");
    errorLabel->setColor(COL_GREEN);
    cardHeaderText.setString("Sign In");
    cardSubText.setString("Please select your role");
    idInput->setText(pid);
    passInput->clear();
    selectedRole = 0; // Patient
}
