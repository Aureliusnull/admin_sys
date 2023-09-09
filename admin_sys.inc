#if defined _adminsys_included
    #endinput
#endif
#define _adminsys_included

/*
    check libs
*/

#if __Pawn >= 0x3A && defined __PawnBuild
    #if __Pawn == 0x3A && __PawnBuild >= 2 || __Pawn > 0x3A
        #define static_enum static enum
    #endif
#endif

#if !defined static_enum
    #define static_enum enum
#endif

#if !defined mysql_included || !defined cache_get_value_name
    #error "Please included <a_mysql.inc> version R40+ before include <admin_sys.inc>" 
#endif

#if !defined _FOREACH_LOCAL_VERSION
    #error "Please included <foreach.inc> before include <admin_sys.inc>"
#endif

#if !defined CMD
    #warning "Please include <Pawn.CMD.inc> or <dc_cmd.inc> to correct work."
#endif

/*
    definitions
*/

#if !defined DB_USERS_ADMINS
    #define DB_USERS_ADMINS "users_admins"
#endif

#if !defined DB_USERS
    #define DB_USERS "users"
#endif

#if !defined HEX_COLOR_ADMIN_CHAT
    #define HEX_COLOR_ADMIN_CHAT 0xBA2821FF
#endif

#if !defined HEX_COLOR_ADMIN_SUCCESS
    #define HEX_COLOR_ADMIN_SUCCESS 0x46BD38ff
#endif

#if !defined HEX_COLOR_ADMIN_WARNING
    #define HEX_COLOR_ADMIN_WARNING 0xAFAFAFFF
#endif 

#if !defined SetPlayerUserID
    #define SetPlayerUserID(%0,%1) g_player_data[%0][E_PLAYER_DATA_ID] = %1
#endif

#if !defined GetPlayerUserID
    #define GetPlayerUserID(%0) g_player_data[%0][E_PLAYER_DATA_ID] 
#endif

#if !defined GetPlayerNameEx
    #define GetPlayerNameEx(%0) g_player_data[%0][E_PLAYER_DATA_NAME] 
#endif

#if !defined MAX_ADMIN_PASSWORD_LENGTH
    #define MAX_ADMIN_PASSWORD_LENGTH 32
#endif

#if !defined MIN_ADMIN_PASSWORD_LENGTH
    #define MIN_ADMIN_PASSWORD_LENGTH 6
#endif

#if !defined MAX_ADMINS_COUNT
	#define MAX_ADMINS_COUNT (50)
#endif

#if !defined MAX_ADMIN_LEVEL
	#define MAX_ADMIN_LEVEL (6)
#endif

#if !defined MIN_ADMIN_LEVEL
	#define MIN_ADMIN_LEVEL (1)
#endif

#if !defined MAX_ADMIN_PASSWORD_HASH_LENGTH
    #define MAX_ADMIN_PASSWORD_HASH_LENGTH (64)
#endif

#if !defined MAX_ADMIN_REG_IP_LENGTH
    #define MAX_ADMIN_REG_IP_LENGTH (16)
#endif

#if !defined MAX_ADMIN_REG_DATE_LENGTH
    #define MAX_ADMIN_REG_DATE_LENGTH (20)
#endif

#if !defined MAX_ADMIN_LOGIN_ATTEMPTS
    #define MAX_ADMIN_LOGIN_ATTEMPTS (5)
#endif

#if !defined MAX_ADMIN_COMMAND_NAME_LENGTH
    #define MAX_ADMIN_COMMAND_NAME_LENGTH (20)
#endif

#if !defined MAX_ADMIN_COMMAND_DESC_LENGTH
    #define MAX_ADMIN_COMMAND_DESC_LENGTH (60)
#endif

#if !defined MAX_ADMIN_COMMAND_ARG_LENGTH
    #define MAX_ADMIN_COMMAND_ARG_LENGTH (60)
#endif

#if !defined DIALOG_ADMIN_NONE
    #define DIALOG_ADMIN_NONE (9999)
#endif

#if !defined DIALOG_ADMIN_REGISTER
    #define DIALOG_ADMIN_REGISTER (6000)
#endif

#if !defined DIALOG_ADMIN_LOGIN
    #define DIALOG_ADMIN_LOGIN (6001)
#endif

#if !defined DIALOG_ADMIN_HELP
    #define DIALOG_ADMIN_HELP (6002)
#endif

#if !defined DIALOG_ADMIN_HELP_RETURN
    #define DIALOG_ADMIN_HELP_RETURN (6003)
#endif

static 
    Iterator: AdminPlayers<MAX_ADMINS_COUNT>,
    MySQL: dbHandle;

static enum {
    ADMIN_COMMAND_LEVEL_0,
    ADMIN_COMMAND_LEVEL_1,
    ADMIN_COMMAND_LEVEL_2,
    ADMIN_COMMAND_LEVEL_3,
    ADMIN_COMMAND_LEVEL_4, 
    ADMIN_COMMAND_LEVEL_5, 
    ADMIN_COMMAND_LEVEL_6
};

static enum E_PLAYER_ADMIN_STRUCT {
    E_PLAYER_ADMIN_ID,
    E_PLAYER_ADMIN_LEVEL, 
    E_PLAYER_ADMIN_PASSWORD[MAX_ADMIN_PASSWORD_HASH_LENGTH + 1],
    E_PLAYER_ADMIN_SALT[MAX_ADMIN_PASSWORD_HASH_LENGTH + 1], 
    E_PLAYER_ADMIN_REG_IP[MAX_ADMIN_REG_IP_LENGTH], 
    E_PLAYER_ADMIN_REG_DATE[MAX_ADMIN_REG_DATE_LENGTH + 1],
    bool: E_PLAYER_ADMIN_LOGGED,
    E_PLAYER_ADMIN_ATTEMPTS 
};
static g_player_admin[MAX_PLAYERS][E_PLAYER_ADMIN_STRUCT];

static const NULL_g_player_admin[E_PLAYER_ADMIN_STRUCT] = {
	0,
	0,
	EOS,
	EOS,
	EOS,
	EOS,
	false,
    MAX_ADMIN_LOGIN_ATTEMPTS
};

static enum E_PLAYER_DATA_STRUCT {
    E_PLAYER_DATA_ID,
    E_PLAYER_DATA_NAME[MAX_PLAYER_NAME + 1]
};
static g_player_data[MAX_PLAYERS][E_PLAYER_DATA_STRUCT];

static const NULL_g_player_data[E_PLAYER_DATA_STRUCT] = {
    0,
    EOS
};

static enum E_ADMIN_COMMAND_STRUCT {
    E_ADMIN_COMMAND_LEVEL,
    E_ADMIN_COMMAND_NAME[MAX_ADMIN_COMMAND_NAME_LENGTH + 1],
    E_ADMIN_COMMAND_ARG[MAX_ADMIN_COMMAND_ARG_LENGTH + 1],
    E_ADMIN_COMMAND_DESC[MAX_ADMIN_COMMAND_DESC_LENGTH + 1]
};

static const g_admin_command[][E_ADMIN_COMMAND_STRUCT] = {
	{ADMIN_COMMAND_LEVEL_1, "/admin", "Текстовое сообщение", "Написать в чат всем администраторам"},
	{ADMIN_COMMAND_LEVEL_1, "/aduty", "Аргументы отсутствуют", "Авторизация/Регистрация в панели администратора"},
	{ADMIN_COMMAND_LEVEL_1, "/ahelp", "Аргументы отсутствуют", "Список команд администора с подробным описанием"},
	{ADMIN_COMMAND_LEVEL_2, "/admins", "Аргументы отсутствуют", "Список администрации в сети"},
    {ADMIN_COMMAND_LEVEL_3, "/off_admins", "Аргументы отсутствуют", "Список администрации (OFFLINE)"},
	{ADMIN_COMMAND_LEVEL_4, "Команды отсутствуют", "Аргументы отсутствуют", "Описание отсутствует"},
	{ADMIN_COMMAND_LEVEL_5, "/create_admin", "ID игрока или часть имени", "Создать нового администратора"},
	{ADMIN_COMMAND_LEVEL_5, "/edit_admin", "ID игрока или часть имени", "Редактировать уровень администратора"},
	{ADMIN_COMMAND_LEVEL_5, "/remove_admin", "ID игрока или часть имени", "Снять администратора"},
	{ADMIN_COMMAND_LEVEL_6, "/off_create_admin", "Имя игрока", "Создать нового администратора (OFFLINE)"},
	{ADMIN_COMMAND_LEVEL_6, "/off_edit_admin", "Имя игрока, Уровень", "Редактировать уровень администратора (OFFLINE)"},
	{ADMIN_COMMAND_LEVEL_6, "/off_remove_admin", "Имя игрока", "Снять администратора (OFFLINE)"}
};

/*  
    forwards
*/

forward bool: admin_IsPlayerLogged(const playerid);
forward bool: admin_IsPlayerValid(const playerid);
forward bool: admin_IsPasswordCharsValid(const string[]);
forward bool: admin_IsPlayerPasswordEqual(password[], const expected_hash[], salt[]);

/*  
    hook OnGameModeInit
*/

public OnGameModeInit()
{
    admin_Init();
    
    #if defined adminsys_OnGameModeInit
        return adminsys_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif

#define OnGameModeInit adminsys_OnGameModeInit
#if defined adminsys_OnGameModeInit
    forward adminsys_OnGameModeInit();
#endif

/*  
	hook OnPlayerConnect
*/

public OnPlayerConnect(playerid)
{ 
    admin_ClearPlayerData(playerid);
    
    GetPlayerName(playerid, g_player_data[playerid][E_PLAYER_DATA_NAME], MAX_PLAYER_NAME);

    admin_LoadPlayerUserID(playerid);

    #if defined adminsys_OnPlayerConnect
        return adminsys_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect adminsys_OnPlayerConnect
#if	defined adminsys_OnPlayerConnect
	forward adminsys_OnPlayerConnect(playerid);
#endif

/* 
    hook OnPlayerDisconnect 
*/

public OnPlayerDisconnect(playerid, reason)
{
    admin_ClearPlayerAdminData(playerid);
        
    #if defined adminsys_OnPlayerDisconnect
        return adminsys_OnPlayerDisconnect(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect adminsys_OnPlayerDisconnect
#if defined adminsys_OnPlayerDisconnect
    forward adminsys_OnPlayerDisconnect(playerid);
#endif

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch (dialogid) {
		case DIALOG_ADMIN_REGISTER: {
            if (!response) {
                return 0;
            }

            if (!(MIN_ADMIN_PASSWORD_LENGTH <= strlen(inputtext) <= MAX_ADMIN_PASSWORD_LENGTH)) {
                SendClientMessage(
                    playerid, 
                    HEX_COLOR_ADMIN_WARNING, 
                    !"Длина пароля должна быть не менее \
                    "#MIN_ADMIN_PASSWORD_LENGTH" и не более \
                    "#MAX_ADMIN_PASSWORD_LENGTH" символов."
                );
                return admin_PlayerRegisterDialog(playerid);
            }

            if (!admin_IsPasswordCharsValid(inputtext)) {
                SendClientMessage(
                    playerid, 
                    HEX_COLOR_ADMIN_WARNING, 
                    !"Пароль может состоять только из латинских символов и цифр (aA-zZ, 0-9)."
                );
                return admin_PlayerRegisterDialog(playerid);
            }
            
            admin_GeneratePlayerPassword(playerid, inputtext);

            admin_SavePlayerEx(playerid, "name", g_player_data[playerid][E_PLAYER_DATA_NAME]);
            admin_SavePlayerEx(playerid, "password", g_player_admin[playerid][E_PLAYER_ADMIN_PASSWORD]);
            admin_SavePlayerEx(playerid, "salt", g_player_admin[playerid][E_PLAYER_ADMIN_SALT]);

            admin_SetPlayerLogged(playerid, true);
            Iter_Add(AdminPlayers, playerid);

            static const message[] = "Ваш новый пароль от панели администратора: {FFFFFF}%s";
            new fmt_message[sizeof(message) + (- 2 + MAX_ADMIN_PASSWORD_LENGTH)];

            format(
                    fmt_message,
                    sizeof(fmt_message),
                    message,
                    inputtext
            );
            SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, fmt_message);
        }
        case DIALOG_ADMIN_LOGIN: {
            if (!response) {
                return 0;
            }

            if (!(MIN_ADMIN_PASSWORD_LENGTH <= strlen(inputtext) <= MAX_ADMIN_PASSWORD_LENGTH)) {
                SendClientMessage(
                    playerid, 
                    HEX_COLOR_ADMIN_WARNING, 
                    !"Пароль должен быть не менее \
                    "#MIN_ADMIN_PASSWORD_LENGTH" и не более \
                    "#MAX_ADMIN_PASSWORD_LENGTH" символов в длину."
                );
                return admin_ShowPlayerLoginDialog(playerid);
            }

            if (!admin_IsPlayerPasswordEqual(inputtext, g_player_admin[playerid][E_PLAYER_ADMIN_PASSWORD], g_player_admin[playerid][E_PLAYER_ADMIN_SALT])) {
                if (--g_player_admin[playerid][E_PLAYER_ADMIN_ATTEMPTS] <= 0) {
                    SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Вы исчерпали все попытки для ввода пароля и были кикнуты.");
                    return Kick(playerid);
                }

                SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Пароли не совпадают. Повторите попытку.");
                return admin_ShowPlayerLoginDialog(playerid);
            } 

            if (!Iter_Contains(AdminPlayers, playerid)) {
                admin_SetPlayerLogged(playerid, true);

                Iter_Add(AdminPlayers, playerid);
            }
           
            SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, !"Вы успешно авторизовались в панели администратора.");
        }
        case DIALOG_ADMIN_HELP: {
            if (!response) {
                return 0;
            }

            new
                string[(20 + MAX_ADMIN_COMMAND_NAME_LENGTH + MAX_ADMIN_COMMAND_DESC_LENGTH + MAX_ADMIN_COMMAND_ARG_LENGTH) * sizeof g_admin_command],
                header_string[50 + (- 2 + 1) + 1];

            string = "Команда\tАргументы\tОписание\n";

            for (new i = 0; i < sizeof g_admin_command; ++i) {
                if (g_admin_command[i][E_ADMIN_COMMAND_LEVEL] != listitem + 1) {
                    continue;
                }

                format(string, sizeof string, "%s%s\t%s\t%s\n",
                        string,
                        g_admin_command[i][E_ADMIN_COMMAND_NAME],
                        g_admin_command[i][E_ADMIN_COMMAND_ARG],
                        g_admin_command[i][E_ADMIN_COMMAND_DESC]
                );
            }

            format(header_string, sizeof header_string, "{5D9978}Команды администратора: {FFFFFF}%d уровня", 
                    admin_GetPlayerLevel(playerid)
            );

            ShowPlayerDialog(
                playerid,
                DIALOG_ADMIN_HELP_RETURN, 
                DIALOG_STYLE_TABLIST_HEADERS,
                header_string,
                string,
                !"Закрыть", !"Назад"
            );
        }
        case DIALOG_ADMIN_HELP_RETURN: {
            if (!response) {
                admin_ShowPlayerHelpDialog(playerid);
            }
        }
    }

    #if defined adminsys_OnDialogResponse
        return adminsys_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnDialogResponse
    #undef OnDialogResponse
#else
    #define _ALS_OnDialogResponse
#endif

#define OnDialogResponse adminsys_OnDialogResponse
#if defined adminsys_OnDialogResponse
    forward adminsys_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif

/*  
    Get / Set
*/

stock admin_GetPlayerAdminID(const playerid) 
{
    return g_player_admin[playerid][E_PLAYER_ADMIN_ID];
}

stock admin_SetPlayerAdminID(const playerid, const value) 
{
    g_player_admin[playerid][E_PLAYER_ADMIN_ID] = value;
}

stock admin_GetPlayerLevel(const playerid)
{
    return g_player_admin[playerid][E_PLAYER_ADMIN_LEVEL];
}

stock admin_SetPlayerLevel(const playerid, const value)
{
    g_player_admin[playerid][E_PLAYER_ADMIN_LEVEL] = value;
}

stock admin_GetPlayerPassword(const playerid, output_string[], size = sizeof(output_string))
{
    strcat((output_string[0] = EOS, output_string), g_player_admin[playerid][E_PLAYER_ADMIN_PASSWORD], size);

    return 1;
}

stock admin_SetPlayerPassword(const playerid, const output_string[], size = sizeof(output_string))
{
    strcat(g_player_admin[playerid][E_PLAYER_ADMIN_PASSWORD], output_string, size);

    return 1;
}

stock admin_GetPlayerSalt(const playerid, output_string[], size = sizeof(output_string))
{
    strcat((output_string[0] = EOS, output_string), g_player_admin[playerid][E_PLAYER_ADMIN_SALT], size);

    return 1;
}

stock admin_SetPlayerSalt(const playerid, const output_string[], size = sizeof(output_string))
{
    strcat(g_player_admin[playerid][E_PLAYER_ADMIN_SALT], output_string, size);

    return 1;
}

stock bool: admin_IsPlayerLogged(const playerid)
{
    return (g_player_admin[playerid][E_PLAYER_ADMIN_LOGGED] != NULL_g_player_admin[E_PLAYER_ADMIN_LOGGED]);
}

stock admin_GetPlayerLogged(const playerid, const bool: is_logged)
{
    return g_player_admin[playerid][E_PLAYER_ADMIN_LOGGED];
}

stock admin_SetPlayerLogged(const playerid, const bool: is_logged)
{
    g_player_admin[playerid][E_PLAYER_ADMIN_LOGGED] = is_logged;
}

stock admin_GetPlayerLoginAttempts(const playerid, const value)
{
    g_player_admin[playerid][E_PLAYER_ADMIN_ATTEMPTS] = value;  
}

stock admin_SetPlayerLoginAttempts(const playerid, const value)
{
    g_player_admin[playerid][E_PLAYER_ADMIN_ATTEMPTS] = value;  
}

/*  
    functions
*/

stock admin_SetMySQLConnectionHandle(MySQL: handle)
{
    dbHandle = handle;
}

stock admin_Init()
{
    new 
        Cache: cache_id = mysql_query(dbHandle, !"SHOW TABLES LIKE '"DB_USERS_ADMINS"'", true),
        tick = GetTickCount();

    if (!cache_num_rows()) {
        mysql_query(dbHandle,
            "\
                CREATE TABLE `"DB_USERS_ADMINS"` ( \
                    `id` int(11) NOT NULL AUTO_INCREMENT, \
                    `user_id` int(11) NOT NULL, \
                    `name` varchar(24) NOT NULL, \
                    `password` varchar(64) NOT NULL, \
                    `salt` varchar(64) NOT NULL, \
                    `level` int(2) NOT NULL, \
                    `register_IP` varchar(16) NOT NULL, \
                    `register_date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP, \
                    PRIMARY KEY (`id`) \
                ) ENGINE=InnoDB DEFAULT CHARSET=utf8; \
            ", false
        );
        mysql_query(dbHandle,
            !"\
                CREATE INDEX \
                    user_id \
                ON \
                    `"DB_USERS_ADMINS"`(user_id) \
            ", false
        );
        mysql_query(dbHandle,
            !"\
                ALTER TABLE \
                    `"DB_USERS_ADMINS"`\
                ADD CONSTRAINT \
                    `users_admins_users_fk_1` \
                FOREIGN KEY \
                    (`user_id`) \
                REFERENCES \
                    `"DB_USERS"` (`id`) \
                ON DELETE CASCADE ON UPDATE CASCADE \
            ", false
        );

        printf("[Debug]: таблица `"DB_USERS_ADMINS"` инициализирована за %d мс.", GetTickCount() - tick);
    } 

    cache_delete(cache_id);

    return mysql_errno() == 0;
}

stock admin_SavePlayerEx(const playerid, const column_name[], const column_value[])
{
    static const query[] = 
    "\
        UPDATE `"DB_USERS_ADMINS"` SET `%e`='%e' WHERE id=%d \
    ";
    
    new fmt_query[sizeof(query) + (- 2 + 20) + (- 2 + MAX_ADMIN_PASSWORD_HASH_LENGTH) + (- 2 + 11)];

    mysql_format(
            dbHandle, 
            fmt_query, 
            sizeof(fmt_query), 
            query, 
            column_name,
            column_value,
            admin_GetPlayerAdminID(playerid)
    );
    mysql_tquery(dbHandle, fmt_query, "", "");

    return mysql_errno() == 0;
}

stock admin_LoadPlayerUserID(playerid)
{
    static const query[] = "SELECT u.id FROM `"DB_USERS"` u WHERE u.name='%e' LIMIT 1";
	
    new fmt_query[sizeof(query) + (- 2 + MAX_PLAYER_NAME)];

    mysql_format(
            dbHandle,
            fmt_query, 
            sizeof(fmt_query),
            query,
            GetPlayerNameEx(playerid)
    );
    mysql_tquery(dbHandle, fmt_query, !"@__OnPlayerLoadUserID", !"d", playerid);

    return mysql_errno() == 0;
}

stock admin_LoadPlayerData(const playerid)
{
	static const query[] = 
	"\
        SELECT * FROM `"DB_USERS_ADMINS"` a \
        LEFT JOIN `"DB_USERS"` u ON u.id=a.user_id \
        WHERE u.id=%d \
	";
	
    new fmt_query[sizeof(query) + (- 2 + 11)];

    mysql_format(
            dbHandle,
            fmt_query, 
            sizeof(fmt_query),
            query,
            GetPlayerUserID(playerid)
    );
    mysql_tquery(dbHandle, fmt_query, !"@__OnPlayerLoadAdminData", !"d", playerid);
    
    return mysql_errno() == 0;
}

stock admin_PlayerCreate(const playerid)
{
    static const query[] = 
    "\
        INSERT INTO `"DB_USERS_ADMINS"` \
        (name, level, register_IP) \
        VALUES \
        ('%e', %d, '%e') \
    ";

    new 
        fmt_query[sizeof(query) + (- 2 + MAX_PLAYER_NAME) + (- 2 + 1) + (- 2 + MAX_ADMIN_REG_IP_LENGTH)],
        tmp_ip[MAX_ADMIN_REG_IP_LENGTH];

    GetPlayerIp(playerid, tmp_ip, MAX_ADMIN_REG_IP_LENGTH);

    admin_SetPlayerLevel(playerid, MIN_ADMIN_LEVEL);

    mysql_format(
            dbHandle, 
            fmt_query, 
            sizeof(fmt_query), 
            query,
            GetPlayerNameEx(playerid),
            MIN_ADMIN_LEVEL,
            tmp_ip
    );
    mysql_tquery(dbHandle, fmt_query, !"@__OnPlayerLoadAdminID", !"d", playerid);

    Iter_Add(AdminPlayers, playerid);

    SendClientMessage(
        playerid, 
        HEX_COLOR_ADMIN_SUCCESS, 
        !"Для регистрации пароля администратора, используйте команду - /aduty."
    );
    return mysql_errno() == 0;
}

stock admin_PlayerUpdate(const playerid)
{
	static const query[] = 
    "\
        UPDATE `"DB_USERS_ADMINS"` SET level=%d WHERE id=%d \
    ";
    
    new fmt_query[sizeof(query) + (- 2 + 1) + (- 2 + 11)];

    format(
            fmt_query, 
            sizeof(fmt_query), 
            query,
            admin_GetPlayerLevel(playerid),
            admin_GetPlayerAdminID(playerid)
    );
    mysql_tquery(dbHandle, fmt_query, "", "");

    SendClientMessage(
        playerid, 
        HEX_COLOR_ADMIN_WARNING, 
        !"Ваш уровень администратора был изменён."
    );
	return mysql_errno() == 0;
}

stock admin_PlayerRemove(const playerid)
{
    static const query[] = 
    "\
        DELETE FROM `"DB_USERS_ADMINS"` WHERE id=%d \
    ";
    
    new fmt_query[sizeof(query) + (- 2 + 11)];

    format(
            fmt_query, 
            sizeof(fmt_query), 
            query,
            admin_GetPlayerAdminID(playerid)
    );
    mysql_tquery(dbHandle, fmt_query, "", "");

    admin_ClearPlayerAdminData(playerid);
    admin_ClearPlayerData(playerid);
    admin_SetPlayerLogged(playerid, false);

    SendClientMessage(
        playerid, 
        HEX_COLOR_ADMIN_WARNING, 
        !"Вы были сняты с должности администратора."
    );
    return mysql_errno() == 0;
}

stock admin_PlayerCreateOffline(const playerid, const name[])
{
	static const query[] =
    "\
    	SELECT u.id, u.name FROM `"DB_USERS"` u WHERE name='%e' LIMIT 1 \
    ";

	new fmt_query[sizeof(query) + (- 2 + MAX_PLAYER_NAME)];

    mysql_format(
            dbHandle,
            fmt_query, 
            sizeof(fmt_query),
            query,
            name
    );
	mysql_tquery(dbHandle, fmt_query, !"@__OnPlayerCreateAdminOffline", !"ds", playerid, name);
	
	return mysql_errno() == 0;
}

stock admin_PlayerEditOffline(const playerid, const name[], const level)
{
	static const query[] =
    "\
    	SELECT ua.name FROM `"DB_USERS_ADMINS"` WHERE name='%e' LIMIT 1 \
    ";

	new fmt_query[sizeof(query) + (- 2 + MAX_PLAYER_NAME)];

    mysql_format(
            dbHandle,
            fmt_query, 
            sizeof(fmt_query),
            query,
            name
    );
	mysql_tquery(dbHandle, fmt_query, !"@__OnPlayerEditAdminOffline", !"dsd", playerid, name, level);
	
	return mysql_errno() == 0;
}

stock admin_PlayerRemoveOffline(const playerid, const name[])
{
    static const query[] =
    "\
    	SELECT ua.name FROM `"DB_USERS_ADMINS"` WHERE name='%e' LIMIT 1 \
    ";

	new fmt_query[sizeof(query) + (- 2 + MAX_PLAYER_NAME)];

    mysql_format(
            dbHandle,
            fmt_query, 
            sizeof(fmt_query),
            query,
            name
    );
	mysql_tquery(dbHandle, fmt_query, !"@__OnPlayerRemoveAdminOffline", !"ds", playerid, name);

    return mysql_errno() == 0;
}

stock admin_ClearPlayerData(const playerid)
{
    g_player_data[playerid] = NULL_g_player_data;
}

stock admin_ClearPlayerAdminData(const playerid)
{
    if (admin_IsPlayerLogged(playerid)) {
        g_player_admin[playerid] = NULL_g_player_admin;

        if (Iter_Contains(AdminPlayers, playerid)) {
            Iter_Remove(AdminPlayers, playerid);
        }
    }

    return 1;
}

stock bool: admin_IsPlayerValid(const playerid) 
{
    return (g_player_admin[playerid][E_PLAYER_ADMIN_LEVEL] != NULL_g_player_admin[E_PLAYER_ADMIN_LEVEL]);
}

stock admin_SendMessage(const color, const message[])
{
    foreach (new idx : AdminPlayers) {
        SendClientMessage(idx, color, message);
    }

    return 1;
}

stock admin_GetAccessCommand(const playerid, const level)
{
    return (admin_GetPlayerLevel(playerid) < level && !admin_IsPlayerLogged(playerid));
}

stock admin_PlayerRegisterDialog(playerid) 
{
    return ShowPlayerDialog(
            playerid, 
            DIALOG_ADMIN_REGISTER, 
            DIALOG_STYLE_INPUT,
            !"{5D9978}Регистрация пароля",
            !"\t{FFFFFF}Придумайте пароль от панели администратора и введите его в диалоговое поле ниже.\n\n\
            {AFAFAF}– Примечание:\n\
            {AFAFAF}•{FFFFFF} Пароль должен быть не менее "#MIN_ADMIN_PASSWORD_LENGTH" и не более "#MAX_ADMIN_PASSWORD_LENGTH" символов в длину.\n\
            {AFAFAF}•{FFFFFF} Пароль может состоять только из латинских символов и цифр (aA-zZ, 0-9).\n\
            {AFAFAF}•{FFFFFF} Пример надёжного пароля: {AFAFAF}32asbe501Tg67FDDDw",
            !"Далее", !"Отмена"
	);
}

stock admin_ShowPlayerLoginDialog(playerid) 
{
    return ShowPlayerDialog(
            playerid, 
            DIALOG_ADMIN_LOGIN, 
            DIALOG_STYLE_PASSWORD,
	    	!"{5D9978}Авторизация администратора",
		    !"{FFFFFF}Введите пароль от панели администратора в диалоговое поле ниже:",
		    !"Далее", !"Отмена"
	);
}

stock admin_ShowPlayerHelpDialog(const playerid) 
{
    new string[(12 + (- 2 + 1)) * MAX_ADMIN_LEVEL + 1];

    for (new i = 0; i < MAX_ADMIN_LEVEL; ++i) {
        format(string, sizeof string, "%s%d уровень\n", 
                string, 
                i + 1
        );
    }

    return ShowPlayerDialog(
            playerid, 
            DIALOG_ADMIN_HELP, 
            DIALOG_STYLE_LIST,
            !"{5D9978}Выбор уровня администратора",
            string,
            !"Выбрать", !"Отмена"
	);
}

stock bool: admin_IsPlayerPasswordEqual(password[], const expected_hash[], salt[])
{
    new hash[MAX_ADMIN_PASSWORD_HASH_LENGTH + 1];

    SHA256_PassHash(password, salt, hash, sizeof hash);

    return bool: (!strcmp(hash, expected_hash, false));
}

stock admin_GeneratePlayerPassword(const playerid, password[]) 
{
	static const random_letters[] = "TOBI8Uv4mZQ5QhuPWRWEtx2WIrqbkginKYfmGdGtyzQcIilwjfx3dkKGpg8RU5";

    new length = strlen(random_letters);
			
    for (new idx = 0; idx < MAX_ADMIN_PASSWORD_HASH_LENGTH; ++idx) {
        g_player_admin[playerid][E_PLAYER_ADMIN_SALT][idx] = random_letters[random(length)];
    }

    SHA256_PassHash(
        password, 
        g_player_admin[playerid][E_PLAYER_ADMIN_SALT], 
        g_player_admin[playerid][E_PLAYER_ADMIN_PASSWORD], 
        MAX_ADMIN_PASSWORD_HASH_LENGTH + 1
    );

	return 1;
}

stock bool: admin_IsPasswordCharsValid(const string[]) 
{
    for (new idx = 0; string[idx] != '\0'; ++idx) {
		switch (string[idx]) {
			case 'a'..'z', 'A'..'Z', '0'..'9': {
				continue;
			}
			default: {
				return false;
			}
		}
	}

    return true;
}

stock admin_GetPlayerOnlineByID(const player_name[])
{
    foreach (new idx : AdminPlayers) {
        if (!strcmp(GetPlayerNameEx(idx), player_name)) {
            return idx;
        }
    }

    return INVALID_PLAYER_ID;
}

/*  
    MySQL callbacks
*/

@__OnPlayerLoadUserID(playerid);
@__OnPlayerLoadUserID(playerid)
{
    new row_count = cache_num_rows();

    if (!row_count) {
        return 0;
    }

    new user_id;
    
    cache_get_value_name_int(0, !"id", user_id);
    
    SetPlayerUserID(playerid, user_id);

    admin_LoadPlayerData(playerid);

    return 1;
}

@__OnPlayerLoadAdminData(playerid);
@__OnPlayerLoadAdminData(playerid)
{
    new row_count = cache_num_rows();

    if (!row_count) {
        return 0;
    }

    cache_get_value_name_int(0, !"id", g_player_admin[playerid][E_PLAYER_ADMIN_ID]);
    cache_get_value_name_int(0, !"level", g_player_admin[playerid][E_PLAYER_ADMIN_LEVEL]);
    
    cache_get_value_name(0, !"password", g_player_admin[playerid][E_PLAYER_ADMIN_PASSWORD], MAX_ADMIN_PASSWORD_HASH_LENGTH + 1);
    cache_get_value_name(0, !"salt", g_player_admin[playerid][E_PLAYER_ADMIN_SALT], MAX_ADMIN_PASSWORD_HASH_LENGTH + 1);
    cache_get_value_name(0, !"register_IP", g_player_admin[playerid][E_PLAYER_ADMIN_REG_IP], MAX_ADMIN_REG_IP_LENGTH);
    cache_get_value_name(0, !"register_date", g_player_admin[playerid][E_PLAYER_ADMIN_REG_DATE], MAX_ADMIN_REG_DATE_LENGTH);
    
    Iter_Add(AdminPlayers, playerid);

    if (isnull(g_player_admin[playerid][E_PLAYER_ADMIN_PASSWORD])) {
        SendClientMessage(
            playerid, 
            HEX_COLOR_ADMIN_WARNING, 
            !"Вы вошли как незарегистрированный администратор."
        );
        SendClientMessage(
            playerid, 
            HEX_COLOR_ADMIN_WARNING, 
            !"Для регистрации пароля используйте - /aduty."
        );
    } else {
        admin_SetPlayerLoginAttempts(playerid, MAX_ADMIN_LOGIN_ATTEMPTS);

        static const message[] = 
        "\
            Вы вошли как администратор (%d) уровня. \
            Для авторизации используйте - /aduty. \
        ";

        new fmt_message[sizeof(message) + (- 2 + 2)];

        format(
                fmt_message, 
                sizeof(fmt_message), 
                message,
                admin_GetPlayerLevel(playerid)
        );
        SendClientMessage(playerid, HEX_COLOR_ADMIN_SUCCESS, fmt_message);
    } 

    return 1;
}

@__OnPlayerLoadAdminID(playerid);
@__OnPlayerLoadAdminID(playerid)
{
    admin_SetPlayerAdminID(playerid, cache_insert_id());
    return 1;
}

@__OnPlayerLoadOfflineAdmins(playerid);
@__OnPlayerLoadOfflineAdmins(playerid)
{
    new row_count = cache_num_rows();

    if (!row_count) {
        return 0;
    }
    
    new
        dialog_string[20 + ((- 2 + MAX_PLAYER_NAME) + (- 2 + 1)) * MAX_ADMINS_COUNT + 1] = "Ник\tУровень\n",
        tmp_name[MAX_PLAYER_NAME + 1], 
        tmp_level;

    for (new idx = 0; idx < row_count; ++idx) {
        cache_get_value_name(idx, !"name", tmp_name);

        if (admin_GetPlayerOnlineByID(tmp_name) != INVALID_PLAYER_ID) {
            continue;
        }

        cache_get_value_name_int(idx, !"level", tmp_level);

        format(
                dialog_string,
                sizeof(dialog_string),
                "%s{5D9978}%s\t{FFFFFF}%d\n",
                dialog_string,
                tmp_name,
                tmp_level
        );
    }

    ShowPlayerDialog(
        playerid, 
        DIALOG_ADMIN_NONE, 
        DIALOG_STYLE_TABLIST_HEADERS,
		!"{5D9978}Администраторы (OFFLINE)",
		dialog_string,
		!"Закрыть", !""
	);
    return 1;
}

@__OnPlayerCreateAdminOffline(playerid, name[]);
@__OnPlayerCreateAdminOffline(playerid, name[])
{
    new row_count = cache_num_rows();

    if (!row_count) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Аккаунт игрока не зарегистрирован в базе данных.");
    }
    
    new user_id;
    
    cache_get_value_name_int(0, !"id", user_id);

    static const query[] = 
    "\
        INSERT INTO `"DB_USERS_ADMINS"` \
            (user_id, name, level) \
        VALUES \
            (%d, '%e', %d) \
    ";
    
    new fmt_query[sizeof(query) + (- 2 + 11) + (- 2 + MAX_PLAYER_NAME) + (- 2 + 1)];
    
    mysql_format(
            dbHandle, 
            fmt_query, 
            sizeof(fmt_query), 
            query,
            user_id,
            name,
            MIN_ADMIN_LEVEL
    );
    mysql_tquery(dbHandle, fmt_query, "", "");
    
    SendClientMessage(
        playerid, 
        HEX_COLOR_ADMIN_SUCCESS, 
        !"Вы успешно создали нового администратора в режиме (OFFLINE)."
    );
    return 1;
}

@__OnPlayerEditAdminOffline(playerid, name[], level);
@__OnPlayerEditAdminOffline(playerid, name[], level)
{
    new row_count = cache_num_rows();

    if (!row_count) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Игрок не является активным администратором.");
    }

    static const query[] = 
    "\
        UPDATE `"DB_USERS_ADMINS"` SET level=%d WHERE name='%e' LIMIT 1 \
    ";
    
    new fmt_query[sizeof(query) + (- 2 + 1) + (- 2 + MAX_PLAYER_NAME)];

    mysql_format(
            dbHandle, 
            fmt_query, 
            sizeof(fmt_query),
            query,
            level,
            name
    );
    mysql_tquery(dbHandle, fmt_query, "", "");

    SendClientMessage(
        playerid, 
        HEX_COLOR_ADMIN_WARNING, 
        !"Вы успешно обновили уровень администратора в режиме (OFFLINE)."
    );
    return 1;
}

@__OnPlayerRemoveAdminOffline(playerid, name[]);
@__OnPlayerRemoveAdminOffline(playerid, name[])
{
    new row_count = cache_num_rows();

    if (!row_count) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Игрок не является активным администратором.");
    }

    static const query[] = 
    "\
        DELETE FROM `"DB_USERS_ADMINS"` WHERE name='%e' LIMIT 1 \
    ";
    
    new fmt_query[sizeof(query) + (- 2 + MAX_PLAYER_NAME)];

    mysql_format(
            dbHandle, 
            fmt_query, 
            sizeof(fmt_query),
            query,
            name
    );
    mysql_tquery(dbHandle, fmt_query, "", "");

    SendClientMessage(
        playerid, 
        HEX_COLOR_ADMIN_WARNING, 
        !"Вы успешно сняли администратора в режиме (OFFLINE)."
    );
    return 1;
}

/*  
    commands
*/

cmd:aduty(playerid) 
{
    if (admin_GetPlayerLevel(playerid) == ADMIN_COMMAND_LEVEL_0) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Вы не обладаете правами администратора.");
    }

    if (Iter_Count(AdminPlayers) > MAX_ADMINS_COUNT) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Превышен лимит администраторов на сервере.");
    }

    if (admin_IsPlayerLogged(playerid)) {
        admin_SetPlayerLogged(playerid, false);
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Вы вышли из панели администратора.");
    }

    if (isnull(g_player_admin[playerid][E_PLAYER_ADMIN_PASSWORD])) {
        admin_PlayerRegisterDialog(playerid);
    } else {
        admin_ShowPlayerLoginDialog(playerid);
    }

    return 1;
}

cmd:ahelp(playerid) 
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_1)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"У Вас недостаточно прав для использования команды.");
    }
    
    admin_ShowPlayerHelpDialog(playerid);
    return 1;
}

cmd:create_admin(playerid, params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_5)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"У Вас недостаточно прав для использования команды.");
    }

    new targetid = INVALID_PLAYER_ID;

    if (sscanf(params, !"u", targetid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, "/create_admin: [ID / Часть имени]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Указанного игрока нет на сервере.");
    }

    if (targetid == playerid) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Вы указали свой ID.");
    }
    
    if (admin_IsPlayerValid(targetid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Указанный игрок уже администратор.");
    }

    if (Iter_Count(AdminPlayers) > MAX_ADMINS_COUNT) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Превышен лимит администраторов на сервере.");
    }

    admin_PlayerCreate(targetid);
	return 1;
}

cmd:edit_admin(playerid, params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_5)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"У Вас недостаточно прав для использования команды.");
    }

    new 
        targetid = INVALID_PLAYER_ID,
        level;

    if (sscanf(params, !"ud", targetid, level)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, "/edit_admin: [ID / Часть имени] [Уровень]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Указанного игрока нет на сервере.");
    }

    if (targetid == playerid) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Вы указали свой ID.");
    }

	if (!admin_IsPlayerValid(targetid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Указанный игрок не администратор.");
    }
    
    if (!(0 <= level <= MAX_ADMIN_LEVEL)) {
    	return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Некорректный уровень администратора.");
    }
    
    if (admin_GetPlayerLevel(targetid) == level) {
    	return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Указанный администратор такого же уровня.");
    }
    
    admin_SetPlayerLevel(playerid, level);
    
    admin_PlayerUpdate(targetid);
    return 1;
}

cmd:remove_admin(playerid, params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_5)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"У Вас недостаточно прав для использования команды.");
    }

    new targetid = INVALID_PLAYER_ID;

    if (sscanf(params, !"u", targetid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, "/remove_admin: [ID / Часть имени]");
    }

    if (targetid == INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Указанного игрока нет на сервере.");
    }

    if (targetid == playerid) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Вы указали свой ID.");
    }
    
    if (!admin_IsPlayerValid(targetid)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Указанный игрок не администратор.");
    }

    admin_PlayerRemove(targetid);
    return 1;
}

cmd:admin(playerid, params[])
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_1)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"У Вас недостаточно прав для использования команды.");
    }

	if (isnull(params)) {
		return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"/admin: [Текст]");
    }

    new string[MAX_CHATBUBBLE_LENGTH + 1];

    format(
            string, 
            sizeof(string), 
            "[A] %s[%d]: %s",
            GetPlayerNameEx(playerid),
            playerid,
            params
    );
    admin_SendMessage(HEX_COLOR_ADMIN_CHAT, string);

	return 1;
}

cmd:admins(playerid) 
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_2)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"У Вас недостаточно прав для использования команды.");
    }

    new 
        dialog_string[27 + ((- 2 + MAX_PLAYER_NAME) + (- 2 + 3) + (- 2 + 1)) * MAX_ADMINS_COUNT + 1] = "Ник\tУровень\n",
        header_string[41 + (- 2 + 2) + 1];

	foreach (new idx : AdminPlayers) {
        format(
                dialog_string,
                sizeof(dialog_string),
                "%s{5D9978}%s[%d]\t{FFFFFF}%d\n",
                dialog_string,
                GetPlayerNameEx(idx),
                idx,
                admin_GetPlayerLevel(idx)
        );
    }

	format(
            header_string, 
            sizeof(header_string), 
            "{BA2821}Администраторы в сети: {FFFFFF}%d",
            Iter_Count(AdminPlayers)
    );

    if (!Iter_Count(AdminPlayers)) {
        strcat((dialog_string[0] = EOS, dialog_string), "{FFFFF}В данный момент нет администраторов в сети.");
    }

	ShowPlayerDialog(
        playerid, 
        DIALOG_ADMIN_NONE, 
        DIALOG_STYLE_TABLIST_HEADERS,
		header_string,
		dialog_string,
		!"Закрыть", !""
	);
	return 1;
}

cmd:off_admins(playerid) 
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_3)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"У Вас недостаточно прав для использования команды.");
    }

    mysql_tquery(
            dbHandle, 
            !"SELECT ua.name, ua.level \
            FROM `"DB_USERS_ADMINS"` ua \
            WHERE ua.level>0", 
            !"@__OnPlayerLoadOfflineAdmins", 
            !"d", 
            playerid
    );
    return 1;
}

#define MAX_PLAYER_NAME_LENGTH 24

cmd:off_create_admin(playerid, params[]) 
{
    if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_6)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"У Вас недостаточно прав для использования команды.");
    }

    new target_name[MAX_PLAYER_NAME_LENGTH];

	if (sscanf(params, "s[" #MAX_PLAYER_NAME_LENGTH "]", target_name)) {
		return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"/off_create_admin: [Имя игрока]");
    }

    if (admin_GetPlayerOnlineByID(target_name) != INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Игрок находится в сети.");
    }

    admin_PlayerCreateOffline(playerid, target_name);
    return 1;
}

cmd:off_edit_admin(playerid, params[]) 
{
	if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_6)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"У Вас недостаточно прав для использования команды.");
    }

    new target_name[MAX_PLAYER_NAME], level;

	if (sscanf(params, "s[" #MAX_PLAYER_NAME_LEN "]d", target_name, level)) {
		return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"/off_edit_admin: [Имя игрока] [Уровень]");
    }

    if (!(0 <= level <= MAX_ADMIN_LEVEL)) {
    	return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Некорректный уровень администратора.");
    }

    if (admin_GetPlayerOnlineByID(target_name) != INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Игрок находится в сети.");
    }

    admin_PlayerEditOffline(playerid, target_name, level);
    return 1;
}

cmd:off_remove_admin(playerid, params[]) 
{
	if (admin_GetAccessCommand(playerid, ADMIN_COMMAND_LEVEL_6)) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"У Вас недостаточно прав для использования команды.");
    }
    
    new target_name[MAX_PLAYER_NAME];

	if (sscanf(params, "s[" #MAX_PLAYER_NAME_LEN "]", target_name)) {
		return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"/off_remove_admin: [Имя игрока]");
    }

    if (admin_GetPlayerOnlineByID(target_name) != INVALID_PLAYER_ID) {
        return SendClientMessage(playerid, HEX_COLOR_ADMIN_WARNING, !"Игрок находится в сети.");
    }

    admin_PlayerRemoveOffline(playerid, target_name);
    return 1;
}

#undef MAX_PLAYER_NAME_LENGTH