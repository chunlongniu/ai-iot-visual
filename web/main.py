import uvicorn
from uvicorn.config import LOGGING_CONFIG
import logging
import apps

logging.basicConfig(format='%(asctime)s %(levelname)s:     %(message)s',
                    datefmt='%Y-%m-%d %H:%M%S',
                    level=logging.DEBUG)


def basic_config(uvicorn_log_config):
    datefmt = '%Y-%m-%d %H:%M%S'
    formatters = uvicorn_log_config['formatters']
    formatters['default']['fmt'] = '%(asctime)s %(levelprefix)s %(message)s'
    formatters['access']['fmt'] = '%(asctime)s %(levelprefix)s %(client_addr)s - "%(request_line)s" %(status_code)s'
    formatters['access']['datefmt'] = datefmt
    formatters['default']['datefmt'] = datefmt
    return uvicorn_log_config


if __name__ == "__main__":
    uvicorn.run("apps:app", host="0.0.0.0", port=8083,
                reload=True, log_config=basic_config(LOGGING_CONFIG))
