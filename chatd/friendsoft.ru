server {
	server_name									friendsoft.ru;
	listen										80;

	expires										max;
	add_header									Strict-Transport-Security "max-age=315360000";

	return										301 https://$server_name$request_uri;
}

server {
	server_name									friendsoft.ru;
	listen										443 ssl;

	ssl_certificate							    /etc/letsencrypt/live/friendsoft.ru/fullchain.pem;
	ssl_certificate_key							/etc/letsencrypt/live/friendsoft.ru/privkey.pem;
    ssl_trusted_certificate                     /etc/letsencrypt/live/friendsoft.ru/chain.pem;

    ssl_stapling                                on;
    ssl_stapling_verify                         on;
    resolver                                    127.0.0.1 8.8.8.8;

	keepalive_timeout							70;

	error_log									/home/www/friendsoft.ru/logs/error.log;
	access_log									off;
	charset										utf-8;
	client_max_body_size						50m;

	root										/home/www/friendsoft.ru/htdocs;


	location = / {
		index									index.html;
	}

	location = /index.html {
		# никаких особых действией не требуется
	}

	location /js {
		expires									max;
	}

	location /css {
		expires									max;
	}

	location /fonts {
		expires									max;
	}

	location /img {
		expires									max;
	}

	# временный локейшен
	location /chat {
		expires									max;
	}

	# динамические ресурсы
	# --------------------------------------------------------------------------

	# сделал отдельно, что не логировать обращение к нему
	location ~ ^/view.json$ {
		proxy_pass								http://localhost:8080;
		access_log								off;
	}

	location ~ \.json$ {
		proxy_pass								http://localhost:8080;
		access_log								/home/www/horn.chat/logs/access.log;
	}

	location ~ ^/(m|n)([0-9]+)\.(jpeg|jpg|png|gif|bmp)$ {

		rewrite ^/(m|n)([0-9]+)\.(.+)$			/image.json?oid=$2&size=$1 last;

		expires									max;
		rewrite_log								on;
		access_log								/home/www/horn.chat/logs/access.log;
	}

	location ~ ^/a([0-9]+)\.(mp3|mp4|aac|mpeg|ogg|wav|mpga)$ {

		rewrite ^/a([0-9]+)\.(.+)$				/audio.json?oid=$1 last;

		expires									max;
		rewrite_log								on;
		access_log								/home/www/horn.chat/logs/access.log;
	}

	location ~ ^/v([0-9]+)\.(3gp|3g2|avi|uvh|uvm|uvu|uvp|uvs|uvv|fvt|f4v|flv|fli|h261|h263|h264|jpm|jpgv|m4v|asf|pyv|wm|wmx|wmv|mj2|mxu|mpeg|mp4|webm|ogv|qt|movie|viv|mov|afl|asx|avs|dif|dl|dv|fmf|gl|isu|m1v|m2v|mjpg|moov|mpa|mpe|mpg|gtc|rv|scm|vdo|vivo|vos|xdr|xsr)$ {

		rewrite ^/v([0-9]+)\.(.+)$				/video.json?oid=$1 last;

		expires									max;
		rewrite_log								on;
		access_log								/home/www/horn.chat/logs/access.log;
	}

	location ~ ^/h([0-9]+)\.zip$ {

		rewrite ^/h([0-9]+)\.zip$				/archive.json?oid=$1 last;

		expires									max;
		rewrite_log								on;
		access_log								/home/www/horn.chat/logs/access.log;
	}

	location ~ ^/i([0-9]+)(\?(.+))?$ {

		rewrite ^/i([0-9]+)(\?(.+))?$			/invite.json?oid=$1$2 last;

		rewrite_log								on;
		access_log								/home/www/horn.chat/logs/access.log;
	}

	location /upload {
		internal;
	}

    location /ws/ {
        proxy_pass                              http://localhost:8090;
        proxy_http_version                      1.1;
        proxy_set_header                        Upgrade $http_upgrade;
        proxy_set_header                        Connection "upgrade";
        proxy_read_timeout                      86400s;
    }
}



server {
	server_name									chat.friendsoft.ru;
	listen										80;

	expires										max;
	add_header									Strict-Transport-Security "max-age=315360000";

	return										301 https://$server_name$request_uri;
}

server {
	server_name									chat.friendsoft.ru;
	listen										443 ssl;

	ssl_certificate							    /etc/letsencrypt/live/friendsoft.ru/fullchain.pem;
	ssl_certificate_key							/etc/letsencrypt/live/friendsoft.ru/privkey.pem;
    ssl_trusted_certificate                     /etc/letsencrypt/live/friendsoft.ru/chain.pem;

    ssl_stapling                                on;
    ssl_stapling_verify                         on;
    resolver                                    127.0.0.1 8.8.8.8;

	keepalive_timeout							70;

	error_log									/home/www/horn.chat/logs/error.log;
	access_log									off;
	charset										utf-8;
	client_max_body_size						20m;

	root										/home/www/horn.chat/htdocs;


	location = / {
		index									index.html;
	}

	location = /index.html {
		# никаких особых действией не требуется
	}

	location /js {
		expires									max;
	}

	location /css {
		expires									max;
	}

	location /fonts {
		expires									max;
	}

	location /img {
		expires									max;
	}

	# временный локейшен
	location /chat {
		expires									max;
	}

	# динамические ресурсы
	# --------------------------------------------------------------------------

	# сделал отдельно, что не логировать обращение к нему
	location ~ ^/view.json$ {
		proxy_pass								http://localhost:8080;
		access_log								off;
	}

	location ~ \.json$ {
		proxy_pass								http://localhost:8080;
		access_log								/home/www/horn.chat/logs/access.log;
	}

	location ~ ^/(m|n)([0-9]+)\.(jpeg|jpg|png|gif|bmp)$ {

		rewrite ^/(m|n)([0-9]+)\.(.+)$			/image.json?oid=$2&size=$1 last;

		expires									max;
		rewrite_log								on;
		access_log								/home/www/horn.chat/logs/access.log;
	}

	location ~ ^/a([0-9]+)\.(mp3|mp4|aac|mpeg|ogg|wav|mpga)$ {

		rewrite ^/a([0-9]+)\.(.+)$				/audio.json?oid=$1 last;

		expires									max;
		rewrite_log								on;
		access_log								/home/www/horn.chat/logs/access.log;
	}

	location ~ ^/v([0-9]+)\.(3gp|3g2|avi|uvh|uvm|uvu|uvp|uvs|uvv|fvt|f4v|flv|fli|h261|h263|h264|jpm|jpgv|m4v|asf|pyv|wm|wmx|wmv|mj2|mxu|mpeg|mp4|webm|ogv|qt|movie|viv|mov|afl|asx|avs|dif|dl|dv|fmf|gl|isu|m1v|m2v|mjpg|moov|mpa|mpe|mpg|gtc|rv|scm|vdo|vivo|vos|xdr|xsr)$ {

		rewrite ^/v([0-9]+)\.(.+)$				/video.json?oid=$1 last;

		expires									max;
		rewrite_log								on;
		access_log								/home/www/horn.chat/logs/access.log;
	}

	location ~ ^/h([0-9]+)\.zip$ {

		rewrite ^/h([0-9]+)\.zip$				/archive.json?oid=$1 last;

		expires									max;
		rewrite_log								on;
		access_log								/home/www/horn.chat/logs/access.log;
	}

	location ~ ^/i([0-9]+)(\?(.+))?$ {

		rewrite ^/i([0-9]+)(\?(.+))?$			/invite.json?oid=$1$2 last;

		rewrite_log								on;
		access_log								/home/www/horn.chat/logs/access.log;
	}

	location /upload {
		internal;
	}

    location /ws/ {
        proxy_pass                              http://localhost:8090;
        proxy_http_version                      1.1;
        proxy_set_header                        Upgrade $http_upgrade;
        proxy_set_header                        Connection "upgrade";
        proxy_read_timeout                      86400s;
    }
}

server {
	server_name									prepod.friendsoft.ru;
    listen										80;

	expires										max;
	add_header									Strict-Transport-Security "max-age=315360000";

	return										301 https://$server_name$request_uri;
}

server {
	server_name									prepod.friendsoft.ru;
	listen										443 ssl;
    
	ssl_certificate							    /etc/letsencrypt/live/friendsoft.ru/fullchain.pem;
	ssl_certificate_key							/etc/letsencrypt/live/friendsoft.ru/privkey.pem;
    ssl_trusted_certificate                     /etc/letsencrypt/live/friendsoft.ru/chain.pem;

    ssl_stapling                                on;
    ssl_stapling_verify                         on;
    resolver                                    127.0.0.1 8.8.8.8;

	keepalive_timeout							70;
    
    proxy_http_version                          1.1;
    proxy_set_header                            Upgrade $http_upgrade;
    proxy_set_header                            Connection "upgrade";

    location / {
        proxy_pass                              http://192.168.30.21:80;
        proxy_set_header                        Host $host;
        proxy_set_header                        X-Real-IP $remote_addr;
        proxy_set_header                        X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header                        X-Forwarded-Proto $scheme;

        proxy_http_version                      1.1;
        proxy_set_header                        Upgrade $http_upgrade;
        proxy_set_header                        Connection "upgrade";
    }
}
