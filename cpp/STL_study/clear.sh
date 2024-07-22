for FILE in *; do
  F=$(file -b ${FILE} |grep ELF)
  if [[ ! -z "${F}" ]]; then
  rm ${FILE}
  fi
done

