CREATE TABLE IF NOT EXISTS pastes (
shortlink text NOT NULL,
expiration_length_in_minutes integer NOT NULL,
creation_at TEXT DEFAULT CURRENT_TIMESTAMP,
paste_path text NOT NULL,
PRIMARY KEY(shortlink)
);
