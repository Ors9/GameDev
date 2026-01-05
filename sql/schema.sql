CREATE TABLE IF NOT EXISTS users (
    id SERIAL PRIMARY KEY,                -- מזהה ייחודי אוטומטי (זה ה-userId)
    username VARCHAR(50) UNIQUE NOT NULL,   -- שם משתמש (חייב להיות ייחודי)
    password_hash TEXT NOT NULL,           -- סיסמה מוצפנת (בשימוש עם libsodium)
    level INT DEFAULT 1,                   -- רמת החשבון הכללית
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP -- תאריך הרשמה
);


CREATE TABLE IF NOT EXISTS UserCharacters (
    cid SERIAL PRIMARY KEY,                -- מזהה ייחודי לדמות (Character ID)
    user_id INT NOT NULL,                  -- מפתח זר המקשר ל-id של טבלת users
    class_type INT NOT NULL,               -- ה-Enum מה-C (לוחם, קוסם וכו')
    cname TEXT NOT NULL,                   -- שם הדמות
    level INT DEFAULT 1,                   -- רמת הדמות
    xp INT DEFAULT 0,                      -- נקודות ניסיון
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    
    -- הגדרת הקשר: אם משתמש נמחק, כל הדמויות שלו נמחקות אוטומטית (CASCADE)
    CONSTRAINT fk_user FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
);

SELECT * FROM users;
SELECT * FROM UserCharacters;
