DROP TABLE IF EXISTS Alarms;
CREATE TABLE IF NOT EXISTS Alarms (
    uuid TEXT PRIMARY KEY,
    sender TEXT,
    created_at DATETIME,
    updated_at DATETIME,
    deleted_at DATETIME DEFAULT NULL
);
